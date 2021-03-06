#include "ck_url_request_windows.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <atlconv.h>
#include <atlstr.h>
#include <windows.h>
#include <winhttp.h>

#include <collection.h>
#include <ppl.h>
#include <ppltasks.h>

#include <QDebug>

using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using namespace Windows::Web::Http;
using namespace concurrency;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;

namespace social_kit {

	typedef std::map<std::string, std::string>::iterator _map_iterator_t;

	class url_request::platform_url_request::private_context {
		public:
			private_context()
				: m_header_data(NULL), m_header_size(0), m_data_buffer(NULL) {}
			~private_context() {
				if (m_header_data != NULL)
					delete[] m_header_data;

				m_header_size = 0;
				qDebug() << Q_FUNC_INFO << "Cleanup";
			}

			void notify_listners(
					url_request::platform_url_request::private_context *a_caller_ref,
					const url_response &a_response) {
				std::for_each(std::begin(m_callback_list), std::end(m_callback_list),
						[&](response_ready_callbcak_t a_func) {
						if (a_func)
						a_func(a_response);
						});
			}

			void parse_http_header();
			bool query_http_data();
			bool get_http_data_block();
			void update_buffer(LPSTR a_buffer, DWORD dwBytesRead);
			void get_http_status_code();

			HttpClient ^ m_http_client;
			task<Windows::Web::Http::HttpResponseMessage ^ > m_hold_task;
			cancellation_token_source *m_task_cancel;
			Uri ^ m_current_uri;

			/* data */
			DWORD m_header_size = 0;
			LPVOID m_header_data = NULL;

			DWORD m_current_block_size;
			DWORD m_total_block_size;
			LPSTR m_data_buffer;

			unsigned int m_status_code;

			std::vector<response_ready_callbcak_t> m_callback_list;
	};

	url_request::platform_url_request::platform_url_request()
		: ctx(new private_context) {
			ctx->m_http_client = ref new Windows::Web::Http::HttpClient();
			ctx->m_http_client->DefaultRequestHeaders->Insert("user-agent", __USER_AGENT_STRING__);
		}

	url_request::platform_url_request::~platform_url_request() {}

	void url_request::platform_url_request::on_response_ready(
			response_ready_callbcak_t a_callback) {
		ctx->m_callback_list.push_back(a_callback);
	}

	static std::string __cx_string_to_string(Platform::String ^ ps) {
		int len = WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), 0, 0,
				NULL, NULL);
		std::string r("", len);
		char *pRBuf = const_cast<char *>(r.c_str());
		WideCharToMultiByte(CP_UTF8, 0, ps->Data(), ps->Length(), pRBuf, len, NULL,
				NULL);
		return r;
	}

	static Platform::String ^ __string_to_cx_string(const std::string &a_data) {
		int len = MultiByteToWideChar(CP_UTF8, 0, a_data.c_str(),
				a_data.length(), NULL, 0);
		auto ps = ref new Platform::String(L"", len);
		wchar_t *pPSBuf = const_cast<wchar_t *>(ps->Data());

		MultiByteToWideChar(CP_UTF8, 0, a_data.c_str(), -1, pPSBuf, len);

		return ps;
	}

	unsigned char *__cx_buffer_to_byte(::Windows::Storage::Streams::IBuffer ^
			a_buffer) {
		if (!a_buffer)
			return 0;

		auto reader = ::Windows::Storage::Streams::DataReader::FromBuffer(a_buffer);
		BYTE *_byte_buffer = new BYTE[a_buffer->Length];

		reader->ReadBytes(
				Platform::ArrayReference<BYTE>(_byte_buffer, a_buffer->Length));
		return (unsigned char *)_byte_buffer;
	}

	void url_request::platform_url_request::append_headers(
			std::map<std::string, std::string> _header_data) {
		for (_map_iterator_t iterator = _header_data.begin();
				iterator != _header_data.end(); iterator++) {
		}
	}


	void url_request::platform_url_request::send_message_async(
			url_request_type_t a_type, const std::string &a_url,
			const url_request_context &a_form_data) {    
		// todo: Check radio state and reachability
		ConnectionProfile ^ _network_profile =
			NetworkInformation::GetInternetConnectionProfile();
		if (a_url.empty() || !_network_profile ||
				_network_profile->GetNetworkConnectivityLevel() !=
				NetworkConnectivityLevel::InternetAccess) {
			return;
		}

		int len = MultiByteToWideChar(CP_UTF8, 0, a_url.c_str(),
				a_url.length(), NULL, 0);
		auto ps = ref new Platform::String(L"", len);
		wchar_t *pPSBuf = const_cast<wchar_t *>(ps->Data());
		MultiByteToWideChar(CP_UTF8, 0, a_url.c_str(), -1, pPSBuf, len);
		Uri ^ current_uri = ref new Uri(ps);

		const char *_mime_type = 0;
		std::map<std::string, std::string> _data = a_form_data.multipart_data();
		std::map<std::string, std::string> _header_data = a_form_data.header();
		std::string _url = a_url + "?" + a_form_data.encode();

		/* update header data */
		for (_map_iterator_t iterator = _header_data.begin();
				iterator != _header_data.end(); iterator++) {
			Platform::String ^_key = __string_to_cx_string(iterator->first);
			Platform::String ^_value = __string_to_cx_string(iterator->second);

			ctx->m_http_client->DefaultRequestHeaders->Insert(_key, _value); 
		}


		/* handle web post */
		if (a_type == kPOSTRequest) {
			qDebug() << Q_FUNC_INFO << "Requesting Post Method";
			try {
				ctx->m_task_cancel = new cancellation_token_source();
				Map<Platform::String^, Platform::String^>^ data =
					ref new Map<Platform::String^, Platform::String^>();

				for (_map_iterator_t iterator = _data.begin();
						iterator != _data.end(); iterator++) {
					Platform::String ^_key = __string_to_cx_string(iterator->first);
					Platform::String ^_value = __string_to_cx_string(iterator->second);
					data->Insert(_key, _value);
				}

				auto content = ref new Windows::Web::Http::HttpFormUrlEncodedContent(data);

				ctx->m_hold_task = create_task(ctx->m_http_client->PostAsync(current_uri, content),
						ctx->m_task_cancel->get_token());

				task<void>([current_uri, this]() {}).then([this](task<void> p_task) {
						ctx->m_hold_task.then([this](Windows::Web::Http::HttpResponseMessage ^
									a_response) {
								if (!a_response || !a_response->Content ||
										!a_response->IsSuccessStatusCode) {
								qDebug() << "Network Backend - UWP WIndows - Server Connect POST "
								".............[FAILED]";
								return;
								}
								auto data_task =
								create_task(a_response->Content->ReadAsBufferAsync());

								data_task.then([this](Windows::Storage::Streams::IBuffer ^ buffer) {
										if (!buffer) {
										qDebug() << "Network Backend - UWP WIndows - Blank Buffer "
										".............[FAILED]";
										return;
										}

										unsigned char *data = __cx_buffer_to_byte(buffer);

										if (!data) {
										qDebug() << "Network Backend - UWP WIndows - Null Data "
										".............[FAILED]";
										return;
										}

										// we are done notify the caller with the data.
										social_kit::url_response response;

										response.set_status_code(200);
										response.set_http_version("HTTP 1.0");
										response.set_data_buffer_size(buffer->Length);
										response.set_data_buffer((const unsigned char *)data,
												buffer->Length);

										ctx->notify_listners(ctx, response);
										delete data;
										qDebug()
											<< "Network Backend - UWP WIndows - Complete .............[ok]";
								});


								});
						});
			} catch (...) {

				qDebug() << Q_FUNC_INFO << "Exception";
			}
			return;
		}

		qDebug() << "Network Backend - Request 01: " << a_url.c_str();


		if (a_type == kGETRequest) {
			try {
				ctx->m_task_cancel = new cancellation_token_source();

				ctx->m_hold_task = create_task(ctx->m_http_client->GetAsync(current_uri),
						ctx->m_task_cancel->get_token());

				task<void>([current_uri, this]() {}).then([this](task<void> p_task) {
						ctx->m_hold_task.then([this](Windows::Web::Http::HttpResponseMessage ^
									a_response) {

								if (!a_response || !a_response->Content ||
										!a_response->IsSuccessStatusCode) {
								qDebug() << "Network Backend - UWP WIndows - Server Connect "
								".............[FAILED]";
								return;
								}

								auto data_task =
								create_task(a_response->Content->ReadAsBufferAsync());

								data_task.then([this](Windows::Storage::Streams::IBuffer ^ buffer) {
										if (!buffer) {
										qDebug() << "Network Backend - UWP WIndows - Blank Buffer "
										".............[FAILED]";
										return;
										}

										unsigned char *data = __cx_buffer_to_byte(buffer);

										if (!data) {
										qDebug() << "Network Backend - UWP WIndows - Null Data "
										".............[FAILED]";
										return;
										}

										// we are done notify the caller with the data.
										social_kit::url_response response;

										response.set_status_code(200);
										response.set_http_version("HTTP 1.1");
										response.set_data_buffer_size(buffer->Length);
										response.set_data_buffer((const unsigned char *)data,
												buffer->Length);

										ctx->notify_listners(ctx, response);
										delete data;
										qDebug()
											<< "Network Backend - UWP WIndows - Complete .............[ok]";
								});
						});
				});
			} catch (std::exception &e) {
				OutputDebugStringA(e.what());
			} catch (Platform::Exception ^ ) {
			}
		}
	}


	void url_request::platform_url_request::send_message_async(
			url_request::url_request_type_t a_type, const std::string &a_message) {
		// todo: Check radio state and reachability
		ConnectionProfile ^ _network_profile =
			NetworkInformation::GetInternetConnectionProfile();
		if (a_message.empty() || !_network_profile ||
				_network_profile->GetNetworkConnectivityLevel() !=
				NetworkConnectivityLevel::InternetAccess) {
			return;
		}

		int len = MultiByteToWideChar(CP_UTF8, 0, a_message.c_str(),
				a_message.length(), NULL, 0);
		auto ps = ref new Platform::String(L"", len);
		wchar_t *pPSBuf = const_cast<wchar_t *>(ps->Data());
		MultiByteToWideChar(CP_UTF8, 0, a_message.c_str(), -1, pPSBuf, len);

		qDebug() << "Network Backend - Request : " << a_message.c_str();

		Uri ^ current_uri = ref new Uri(ps);

		if (a_type == kGETRequest) {
			try {
				ctx->m_task_cancel = new cancellation_token_source();

				ctx->m_hold_task = create_task(ctx->m_http_client->GetAsync(current_uri),
						ctx->m_task_cancel->get_token());

				task<void>([current_uri, this]() {}).then([this](task<void> p_task) {
						ctx->m_hold_task.then([this](Windows::Web::Http::HttpResponseMessage ^
									a_response) {

								if (!a_response || !a_response->Content ||
										!a_response->IsSuccessStatusCode) {
								qDebug() << "Network Backend - UWP WIndows - Server Connect "
								".............[FAILED]";
								return;
								}

								auto data_task =
								create_task(a_response->Content->ReadAsBufferAsync());

								data_task.then([this](Windows::Storage::Streams::IBuffer ^ buffer) {
										if (!buffer) {
										qDebug() << "Network Backend - UWP WIndows - Blank Buffer "
										".............[FAILED]";
										return;
										}

										unsigned char *data = __cx_buffer_to_byte(buffer);

										if (!data) {
										qDebug() << "Network Backend - UWP WIndows - Null Data "
										".............[FAILED]";
										return;
										}

										// we are done notify the caller with the data.
										social_kit::url_response response;

										response.set_status_code(200);
										response.set_http_version("HTTP 1.1");
										response.set_data_buffer_size(buffer->Length);
										response.set_data_buffer((const unsigned char *)data,
												buffer->Length);

										ctx->notify_listners(ctx, response);
										delete data;
										qDebug()
											<< "Network Backend - UWP WIndows - Complete .............[ok]";
								});
						});
				});
			} catch (std::exception &e) {
				OutputDebugStringA(e.what());
			} catch (Platform::Exception ^ ) {
			}
		}
	}

	void url_request::platform_url_request::private_context::parse_http_header() {}

	bool url_request::platform_url_request::private_context::query_http_data() {
		return FALSE;
	}

	bool url_request::platform_url_request::private_context::get_http_data_block() {
		return FALSE;
	}

	void url_request::platform_url_request::private_context::update_buffer(
			LPSTR a_buffer, DWORD dwBytesRead) {}

	void
		url_request::platform_url_request::private_context::get_http_status_code() {}
}

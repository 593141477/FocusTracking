// Author: gzp

#include "httpserver.h"
#include "tasks.h"
#include "common.h"

#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>
#include <time.h>
#include <chrono>
#include <string>
#include <cstring>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#define PORT            8888
#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     20
#define MAXANSWERSIZE   512
#define GET             0
#define POST            1
using namespace std;
struct connection_info_struct {
    int connectiontype;
    char *answerstring;
    struct MHD_PostProcessor *postprocessor;
};
const char *greetingpage =
    "<html><body><h1>Welcome, %s!</center></h1></body></html>";
const char *errorpage =
    "<html><body>This doesn’t seem to be right.</body></html>";

string s;
static int send_page (struct MHD_Connection *connection, const char *page)
{
    int ret;
    struct MHD_Response *response;
    response =
        MHD_create_response_from_buffer (strlen (page), (void *) page,
                                         MHD_RESPMEM_MUST_COPY);
    if (!response)
        return MHD_NO;

    MHD_add_response_header(response, "Content-Type", "text/html");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "*");
    MHD_add_response_header(response, "Access-Control-Allow-Origin", "http://115.28.18.88:80");
    ret = MHD_queue_response (connection, MHD_HTTP_OK, response);
    MHD_destroy_response (response);
    return ret;
}
static int iterate_post (void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
                         const char *filename, const char *content_type,
                         const char *transfer_encoding, const char *data, uint64_t off,
                         size_t size)
{
    return MHD_NO;
}
static void request_completed (void *cls, struct MHD_Connection *connection,
                               void **con_cls, enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;
    if (NULL == con_info)
        return;
    if (con_info->connectiontype == POST) {
        MHD_destroy_post_processor (con_info->postprocessor);
    }
    free (con_info);
    *con_cls = NULL;
}

std::chrono::system_clock::time_point date_string_to_time_point(const string &s)
{
    std::tm tm;
    strptime(s.c_str(), "%Y-%m-%d", &tm);
    return std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

string get_info(const char *todo, size_t len)
{
    using namespace std::chrono;
    string first, second(todo, len);
    first = second.substr(0, second.find('+'));
    second = second.substr(second.find('+') + 1);
    log_info("Fetch data from [%s] to [%s]", first.c_str(), second.c_str());

    auto s = date_string_to_time_point(first);
    auto t = date_string_to_time_point(second);

    // std::time_t ss = std::chrono::system_clock::to_time_t(s);
    // std::cout << std::ctime(&ss) << std::endl;
    // std::time_t tt = std::chrono::system_clock::to_time_t(t);
    // std::cout << std::ctime(&tt) << std::endl;

    return GetBundleTitlesByDate(system_clock::now()-hours(48), system_clock::now());
}

static int answer_to_connection (void *cls, struct MHD_Connection *connection,
                                 const char *url, const char *method,
                                 const char *version, const char *upload_data,
                                 size_t *upload_data_size, void **con_cls)
{
    if (NULL == *con_cls) {
        struct connection_info_struct *con_info;
        con_info = (connection_info_struct *)malloc (sizeof (struct connection_info_struct));
        if (NULL == con_info)
            return MHD_NO;
        con_info->answerstring = NULL;
        if (0 == strcmp (method, "POST")) {
            con_info->postprocessor =
                MHD_create_post_processor (connection, POSTBUFFERSIZE,
                                           iterate_post, (void *) con_info);
            if (NULL == con_info->postprocessor) {
                free (con_info);
                return MHD_NO;
            }
            con_info->connectiontype = POST;
        } else
            con_info->connectiontype = GET;
        *con_cls = (void *) con_info;
        return MHD_YES;
    }
    if (0 == strcmp (method, "GET")) {
        ifstream fin("index.html");
        string s = "", ss;
        while (getline(fin, ss)) {
            s += ss + "\n";
        }
        fin.close();
        return send_page (connection, s.c_str());
    }
    if (0 == strcmp (method, "POST")) {
        struct connection_info_struct *con_info = (connection_info_struct *)*con_cls;
        if (*upload_data_size != 0) {
            MHD_post_process (con_info->postprocessor, upload_data,
                              *upload_data_size);
            s = get_info(upload_data, *upload_data_size);
            // cout << s << endl;
            *upload_data_size = 0;
            return MHD_YES;
        }
        return send_page(connection, s.c_str());
    }
    return send_page (connection, errorpage);
}

//单例模式
static struct MHD_Daemon *gDaemon;
void http_server_start()
{
    gDaemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                               &answer_to_connection, NULL,
                               MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                               NULL, MHD_OPTION_END);

}
void http_server_stop()
{
    if (gDaemon)
        MHD_stop_daemon (gDaemon);
}
#if 0
int main ()
{
    struct MHD_Daemon *daemon;
    daemon = MHD_start_daemon (MHD_USE_SELECT_INTERNALLY, PORT, NULL, NULL,
                               &answer_to_connection, NULL,
                               MHD_OPTION_NOTIFY_COMPLETED, request_completed,
                               NULL, MHD_OPTION_END);
    if (NULL == daemon)
        return 1;
    getchar ();
    MHD_stop_daemon (daemon);
    return 0;
}
#endif
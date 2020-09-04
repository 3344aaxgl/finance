#include <curl/curl.h>
#include <iostream>
#include <string>

const char* url = "http://api.tushare.pro";
const char* token = "654f4247efbc88c97f270f9226fe129819ad07b36b3452a137a2324b";
class Data
{
  public:
    static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
    {
        std::string* response = static_cast<std::string*>(userdata);
        size_t realsize = size * nmemb;
        response->append(ptr, realsize);
        return realsize;
    }

    Data()
    {
        curl = nullptr;
    }

    ~Data()
    {
        if (curl)
            curl_easy_cleanup(curl);
    }
    void InitCurl()
    {
        curl = curl_easy_init();
        if (!curl) {
            std::cout << "Failed to Init curl" << std::endl;
            return;
        }
    }

    void PerformCurl(const char* url, const char* api_name, const char* token, const char* params, const char* fields)
    {
        std::string response;
        std::string postdata;
        struct curl_slist* list = NULL;
        curl_easy_setopt(curl, CURLOPT_URL, url);
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        postdata.append("{\"api_name\": \"")
            .append(api_name)
            .append("\", \"token\": \"")
            .append(token)
            .append("\", \"params\": ")
            .append(params)
            .append(", \"fields\": \"")
            .append(fields)
            .append("\"}");
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postdata.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Data::write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, static_cast<void*>(&response));
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
        std::cout<<"postdata:"<<postdata<<std::endl;
        CURLcode res = curl_easy_perform(curl);
        curl_slist_free_all(list);
        if(res != CURLE_OK)
        {
            std::cout<<"Failed to do curl perform, error message:"<<curl_easy_strerror(res)<<std::endl;
            return;
        }
        std::cout<<"response:"<<response<<std::endl;
    }

  private:
    CURL* curl;
};

int main()
{
    Data data;
    data.InitCurl();
    data.PerformCurl(url, "stock_basic", token, "{\"list_stauts\":\"L\"}", "ts_code,name,area,industry,list_date");
    return 0;
}

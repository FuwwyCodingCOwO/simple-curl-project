#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct memory{
    char *response;
    size_t size;
};


static size_t cb(char *data, size_t size, size_t nmemb, void *clientp){
    size_t realsize = size * nmemb;
    struct memory *mem = (struct memory *)clientp;

    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if (!ptr){
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}


int main(){
    CURL *curl_handle = curl_easy_init();

    if (curl_handle){
        struct memory chunk = {0};
        CURLcode res;
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, cb);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_URL, "https://kernel.org");

        res = curl_easy_perform(curl_handle);

        if (!res){
            FILE *file = fopen("curl_output", "w+");
            if (file){
                fwrite(chunk.response, sizeof(char), chunk.size, file);
                fclose(file);
            }
            else{
                fclose(file);
                curl_easy_cleanup(curl_handle);
                return 3;
            }
        }
        else{
            curl_easy_cleanup(curl_handle);
            return 2;
        }

        free(chunk.response);
        curl_easy_cleanup(curl_handle);
        return 0;
    }
    else{
        return 1;
    }
}

















// int _main(void){
//     CURL *curl = curl_easy_init();
//     if (curl){
//         CURLcode res;
//         curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION);
//         curl_easy_setopt(curl, CURLOPT_URL, "https://kernel.org");
//         res = curl_easy_perform(curl);
//
//         if (!res){
//             char *ct = NULL;
//             res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
//             if (!res && ct){
//                 printf("Content-Type: %s\n", ct);
//             }
//         }
//
//         curl_easy_cleanup(curl);
//         return 0;
//     }
//     else {
//         return 1;
//     }
// }

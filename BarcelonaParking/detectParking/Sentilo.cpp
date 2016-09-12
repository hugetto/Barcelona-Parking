
#include "stdafx.h"

#include "Sentilo.h"

#define DEVICE_ID "PK0001"
#define IDENTITY_KEY_HEADER "IDENTITY_KEY: 189b0f206d0abd8603343fac094be0d4009d66a81437f9a81136205bf7ab54b0"
#define BASE_URL "https://api-sentilo.diba.cat"
#define PARTICIPANT "participant002"
#define SENSOR_NAME "PK0001"

struct MemoryStruct {
	char *ptr;
	size_t len;
};

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct MemoryStruct *s)
{
	size_t new_len = s->len + size*nmemb;
	s->ptr = (char*)realloc(s->ptr, new_len + 1);
	if (s->ptr == NULL) {
		fprintf(stderr, "realloc() failed\n");
		exit(EXIT_FAILURE);
	}
	memcpy(s->ptr + s->len, ptr, size*nmemb);
	s->ptr[new_len] = '\0';
	s->len = new_len;

	return size*nmemb;
}

void Sentilo::UnregisterFromSentilo()
{
	//Before start, unregister and re-register from sentilo.diba.cat
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, IDENTITY_KEY_HEADER);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		std::string url = BASE_URL + std::string("/catalog/") + PARTICIPANT;

		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		std::ifstream in("register.json");
		std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents.c_str());

		// example.com is redirected, so we tell libcurl to follow redirection 
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//Do not verify CA certificates
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		//Return data from HTTP request
		struct MemoryStruct chunkData;
		chunkData.ptr = (char*)malloc(1);  /* will be grown as needed by the realloc above */
		chunkData.len = 0;    /* no data at this point */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunkData);

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

		// Perform the request, res will get the return code 
		res = curl_easy_perform(curl);
		// Check for errors 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		printf("%s \n", chunkData.ptr);

		// always cleanup 
		curl_easy_cleanup(curl);
	}
}

void Sentilo::RegisterToSentilo()
{
	//Before start, unregister and re-register from sentilo.diba.cat
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, IDENTITY_KEY_HEADER);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		std::string url = BASE_URL + std::string("/catalog/") + PARTICIPANT;
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		// Now specify we want to POST data
		curl_easy_setopt(curl, CURLOPT_POST, 1L);


		std::ifstream in("register.json");
		std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, contents.c_str());

		// example.com is redirected, so we tell libcurl to follow redirection 
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//Do not verify CA certificates
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		//Return data from HTTP request
		struct MemoryStruct chunkData;
		chunkData.ptr = (char*)malloc(1);  /* will be grown as needed by the realloc above */
		chunkData.len = 0;    /* no data at this point */
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunkData);

		// Perform the request, res will get the return code 
		res = curl_easy_perform(curl);
		// Check for errors 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));


		printf("%s \n", chunkData.ptr);

		// always cleanup 
		curl_easy_cleanup(curl);
	}
}


Sentilo::Sentilo()
{

}


Sentilo::~Sentilo()
{

}

void Sentilo::Init()
{
	//UnregisterFromSentilo();
	RegisterToSentilo();
}


void Sentilo::SendData(float data)
{
	//Before start, unregister and re-register from sentilo.diba.cat
	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *chunk = NULL;
		chunk = curl_slist_append(chunk, IDENTITY_KEY_HEADER);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

		std::string url = BASE_URL + std::string("/data/") + PARTICIPANT  + std::string("/") + SENSOR_NAME + std::string("/") + std::to_string(data);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

		// example.com is redirected, so we tell libcurl to follow redirection 
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

		//Do not verify CA certificates
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

		// Perform the request, res will get the return code 
		res = curl_easy_perform(curl);
		// Check for errors 
		if (res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
				curl_easy_strerror(res));

		// always cleanup 
		curl_easy_cleanup(curl);
	}
}



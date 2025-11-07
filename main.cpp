#include <curl/curl.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include "openssl/hmac.h"
#include "keys.h"
#include "realtime.h"
#include <deque>
#include <thread>


using json = nlohmann::json;

std::string api = KEYS::api_key;
std::string secret = KEYS::api_secret;

std::string hmac_sha256(const std::string &msg, const std::string &key) {
    unsigned char* digest;
    digest = HMAC(EVP_sha256(),
                  key.data(), key.size(),
                  (unsigned char*)msg.data(), msg.size(),
                  nullptr, nullptr);

    // Convert to hex string
    char mdString[65];
    for (int i = 0; i < 32; i++)
        sprintf(&mdString[i * 2], "%02x", (unsigned int)digest[i]);

    return std::string(mdString);
}


int main() {
    MatrixXd voyah = MatrixXd::Zero(8, 2);
    VectorXd boyah = VectorXd::Zero(8);
    voyah << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16;
    boyah << 1, 2, 3, 4, 5, 6, 7, 8;
    Irlregression clepsidra;
    auto bongo = clepsidra.recalc(voyah, boyah);
    std::cout << bongo << std::endl;

    std::cout << "j";
    DynamicPrice DP;
    std::cout << "j";
    DP.connector("https://api.bybit.com/v5/market/recent-trade?category=linear&symbol=TONUSDT&limit=1");
    std::cout << "j";
    DP.start();
    std::thread fetch_thread([&DP]() {
       DP.loop();
   });
    std::cout << "j";
    while (true) {
        std::cout << DP.quantities.back();
        std::this_thread::sleep_for(std::chrono::seconds(0));
    }
    fetch_thread.join();
}
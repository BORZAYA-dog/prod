//
// Created by sofab on 15.09.2025.
//

#ifndef PROD_REALTIME_H
#define PROD_REALTIME_H

#include <curl/curl.h>
#include <ctime>
#include <chrono>
#include <iostream>
#include <string>
#include "nlohmann/json.hpp"
#include <deque>
#include <mutex>
#include <thread>
#include "Eigen/Dense"

using json = nlohmann::json;
using Eigen::MatrixXd;
using Eigen::VectorXd;


class DynamicPrice {
public:
    static const size_t N = 10;
    std::deque<double> prices = std::deque<double>(N);
    std::deque<double> quantities = std::deque<double>(N);
    std::mutex mtx;

    DynamicPrice() : handle(nullptr), is_on(false) {}
    void connector(const std::string &url) {
        handle = curl_easy_init();
        if (!handle) {
            throw std::runtime_error("Failed to initialize CURL");
        }
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, &turtle);  // <-- use member, not local
    }

    void start() {
        is_on = true;
    };
    void stop() {
        is_on = false;
    };
    void loop() {
        while (is_on) {
            turtle.clear();
            curl_easy_perform(handle);
            if (!turtle.empty()) {
                json j = json::parse(turtle, /*callback=*/nullptr, /*allow_exceptions=*/true);
                const auto& list = j["result"]["list"];
                const auto& item = list.front();
                double drum = std::stod(item["price"].get_ref<const std::string&>());
                double hack = std::stod(item["size"].get_ref<const std::string&>());
                std::lock_guard<std::mutex> lock(mtx);
                prices.push_back(drum);
                quantities.push_back(hack);
            }
            else {
                prices.push_back(0);
                quantities.push_back(0);
            };
            std::this_thread::sleep_for(std::chrono::milliseconds(0));

        }
    }
    private:
        CURL* handle;
        bool is_on = false;
        std::string turtle;
    static size_t write_data(char* ptr, size_t sz, size_t nm, void* userdata) {
        auto* out = static_cast<std::string*>(userdata);
        out->append(ptr, sz*nm);
        return sz*nm;
    }

};



class Irlregression {
    public:
    MatrixXd X;
    VectorXd Y;
    Irlregression() {}
    VectorXd recalc(const MatrixXd& X, const VectorXd& Y) {
        VectorXd betas;
        MatrixXd xt = X.transpose();
        betas = (xt * X).inverse() * xt * Y;
        return betas;
    }
};

#endif //PROD_REALTIME_H
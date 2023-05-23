/**  ____   _____   ______  _   _            _____          _____   _    _  _____
 *  / __ \ |  __ \ |  ____|| \ | |    /\    |_   _|        |  __ \ | |  | ||  __ \
 * | |  | || |__) || |__   |  \| |   /  \     | |   ______ | |__) || |__| || |__) |
 * | |  | ||  ___/ |  __|  | . ` |  / /\ \    | |  |______||  ___/ |  __  ||  ___/
 * | |__| || |     | |____ | |\  | / ____ \  _| |_         | |     | |  | || |
 *  \____/ |_|     |______||_| \_|/_/    \_\|_____|        |_|     |_|  |_||_|
 *
 * OpenAI PHP extension                             Copyright (c) 2023 Mariusz Duka
 * version 1.0.0                                                https://duka.pl/en/
 * https://openai-php.com
 * https://github.com/mariuszduka/openai-php                  The MIT License (MIT)
 */

#include <phpcpp.h>                     // PHP-CPP library
#include "libs/openai-cpp/openai.hpp"   // olrea/openai-cpp
#include <fstream>

#define PHP_OPENAI_VERSION              "1.0.0"
#define PHP_OPENAI_NAME                 "OpenAI"
#define PHP_OPENAI_WEBPAGE              "https://openai-php.com"
#define PHP_OPENAI_GITHUB               "https://github.com/mariuszduka/openai-php"

#define PHP_OPENAI_RESPONSE_ARRAY       11
#define PHP_OPENAI_RESPONSE_JSON        12

#define PHP_OPENAI_IMAGE_CREATE         21
#define PHP_OPENAI_IMAGE_EDIT           22
#define PHP_OPENAI_IMAGE_VARIATION      23

#define PHP_OPENAI_AUDIO_TRANSCRIPTION  31
#define PHP_OPENAI_AUDIO_TRANSLATION    32

#define PHP_OPENAI_FILE_LIST            41
#define PHP_OPENAI_FILE_UPLOAD          42
#define PHP_OPENAI_FILE_DELETE          43
#define PHP_OPENAI_FILE_RETRIEVE        44
#define PHP_OPENAI_FILE_CONTENT         45

#define PHP_OPENAI_FINETUNE_CREATE      51
#define PHP_OPENAI_FINETUNE_LIST        52
#define PHP_OPENAI_FINETUNE_RETRIEVE    53
#define PHP_OPENAI_FINETUNE_CANCEL      54
#define PHP_OPENAI_FINETUNE_EVENTS      55
#define PHP_OPENAI_FINETUNE_DELETE      56

#define PHP_OPENAI_LOG_ENABLE           0
#define PHP_OPENAI_LOG_FILE             "./openai.log"

#define PHP_OPENAI_TEST                 false // use example response data
#define PHP_OPENAI_DEBUG                false // show debug info

#define INFO_FUNC(module) void info__##module(Php::Extension* extension)
#define CALL_INFO_FUNC(module, ex) info__##module(ex);

#if PHP_OPENAI_TEST
/*
 * Load sample response data.
 * If true then communication with the OpenAI server is inactive.
 */
#include "libs/openai-responses.h"
#endif

/**
 * OpenAI class.
 */
class OpenAI : public Php::Base
{
private:    
    using json = nlohmann::json;

    openai::OpenAI openai_instance{};
    short int log_enable = 0;

    /**
     * Convert json object to array.
     *
     * @param   items   json object
     * @return          array converted from json object
     */
    inline Php::Value to_array(nlohmann::json& items)
    {
        Php::Value array = nullptr;
        std::string valString = "";

        for (const auto& val : items.items())
        {
#if PHP_OPENAI_DEBUG
            std::cout << "  " << val.key() << ": " << val.value() << "\n";
            std::cout << "  " << val.key() << ": " << val.value().type_name() << "\n";
#endif
            if (val.value().is_structured())
            {
                array[val.key()] = (val.value().is_array() && val.value().size() == 1) ? to_array(val.value()[0]) : to_array(val.value());
            }
            else {
                if (val.value().is_string())
                {
                    valString = val.value().dump();
                    valString = valString.substr(1, valString.size() - 2);

                    array[val.key()] = valString;
                }
                else {
                    array[val.key()] = val.value().dump();
                }                
            }
        }

        return array;
    }

    /**
     * Check if the key is in the array.
     *
     * @param   value   key name in the table
     * @param   array   the array in which we are looking for the key
     * @return          true or false
     */
    inline bool in_array(const std::string& value, const std::vector<std::string>& array)
    {
        return std::find(array.begin(), array.end(), value) != array.end();
    }

    /**
     * Check if string is integer.
     *
     * @param   s   string that we are testing
     * @return      true or false
     */
    inline bool is_integer(const std::string& s)
    {
        if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        {
            return false;
        }

        char* p;
        strtol(s.c_str(), &p, 10);

        return (*p == 0);
    }
    
    /**
     * Check if the string is a floating point number.
     *
     * @param   s   string that we are testing
     * @return      true or false
     */
    inline bool is_float(const std::string& s)
    {
        std::string::const_iterator it = s.begin();
        bool decimalPoint = false;
        unsigned int minSize = 0;
        if (s.size() > 0 && (s[0] == '-' || s[0] == '+')) 
        {
            it++;
            minSize++;
        }
        while (it != s.end()) 
        {
            if (*it == '.') 
            {
                if (!decimalPoint) decimalPoint = true;
                else break;
            }
            else if (!std::isdigit(*it) && ((*it != 'f') || it + 1 != s.end() || !decimalPoint)) 
            {
                break;
            }
            ++it;
        }
        return s.size() > minSize && it == s.end();
    }

    /**
     * Response if the parameter is required.
     *
     * @param   request the name of the parameter in the request
     * @return          string
     */
    inline Php::Value getErrorReqMessage(std::string request)
    {
        return "The '" + request + "' parameter is required in the request body.";
    }

    /**
     * Response if the parameter is invalid.
     *
     * @param   request the name of the parameter in the request
     * @return          string
     */
    inline Php::Value getErrorInvMessage(std::string request)
    {
        return "The '" + request + "' parameter is invalid in the request body.";
    }

    /**
     * Get current date/time.
     * 
     * @return  string  format YYYY-MM-DD.HH:mm:ss
     */
    inline const std::string currentDateTime() 
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

        return buf;
    }

    /**
     * Saving logs in a text file.
     *
     * @param   log string to be written to the logs
     */
    inline void appendLogFile(std::string& log)
    {
        std::string filepath = "";
        std::vector<std::string> files = {};

        if (log_enable)
        {
            std::string filepath = Php::ini_get("openai.log_filepath");
            if (filepath == "")
            {
                filepath = PHP_OPENAI_LOG_FILE;
            }
            files.push_back(filepath);
        }

#ifdef PHP_WIN32
        if (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1)
#else
        if (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1)
#endif
        {
#ifdef PHP_WIN32
            std::string filepath = (Php::defined("OPENAI_LOG_FILEPATH")) ? Php::call("constant", "OPENAI_LOG_FILEPATH") : "";
#else
            std::string filepath = (Php::defined("OPENAI_LOG_FILEPATH")) ? Php::constant("OPENAI_LOG_FILEPATH") : "";
#endif

            if (filepath == "")
            {
                filepath = PHP_OPENAI_LOG_FILE;
            }
            files.push_back(filepath);
        }

        std::fstream file;

        for (unsigned int i = 0; i < files.size(); i++)
        {
            filepath = files[i];

            file.open(filepath, std::ios::out | std::ios::app);
            if (file.fail())
            {
                throw Php::Exception("There was a problem with saving logs in the " + filepath + " file.");
            }
            else
            {
                file << log << std::endl;
                file.close();
            }
        }
    }

    /**
     * Saving logs in a text file.
     *
     * @param   request     the content of the request
     * @param   response    the content of the response
     */
    inline void saveLog(std::string request, std::string response)
    {
        std::string log = "[" + currentDateTime() + "]\nRequest:\n" + request + "\n[" + currentDateTime() + "]\nResponse:\n" + response + "\n";
        appendLogFile(log);
#if PHP_OPENAI_DEBUG
        std::cout << log << "\n";
#endif
    }

public:
    /**
     * C++ Constructor
     */
    OpenAI() = default;

    /**
     * C++ Destructor
     */
    virtual ~OpenAI() = default;

    /**
     * PHP "Constructor"
     * 
     * @param   params
     */
    void __construct(Php::Parameters& params)
    {
        log_enable = Php::ini_get("openai.log_enable");

        if (!params.empty())
        {
            Php::Value api_key = params[0];
            Php::Value org_id = (count(params) > 1) ? params[1] : "";

            openai_instance.setToken(api_key, org_id);
            openai_instance.setThrowException(true);
        }
    }

    /**
     * OpenAI: List models
     * https://platform.openai.com/docs/api-reference/models/list
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value models(Php::Parameters& params)
    {
        short int response = (!params.empty()) ? (short int)params[0] : PHP_OPENAI_RESPONSE_ARRAY;
        
        try {
#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_MODELS);
#else
            auto c = openai_instance.model.list();
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog("{}", c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }

        }
        catch (Php::Exception& exception)
        {
            throw Php::Exception(exception.what());
        }
    }

    /**
     * OpenAI: Retrieve model
     * https://platform.openai.com/docs/api-reference/models/retrieve
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value model(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Value model = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_MODEL);
#else
            auto c = openai_instance.model.retrieve(model);
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog("{}", c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Completions
     * https://platform.openai.com/docs/api-reference/completions
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value completion(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

            if (!request["model"]) {
                throw Php::Exception(getErrorReqMessage("model"));
            }

            std::vector<std::string> n{ "max_tokens", "temperature", "top_p", "n", "logprobs", "presence_penalty", "frequency_penalty", "best_of" };
            std::vector<std::string> b{ "stream", "echo" };
            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                    else {
                        j[iter.first] = nullptr;
                    }

                }
                else if (in_array(iter.first, b))
                {
                    j[iter.first] = iter.second == "true" ? true : false;
                }
                else {
                    j[iter.first] = iter.second;
                }
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_COMPLETION);
#else
            auto c = openai_instance.completion.create(j);
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Chat
     * https://platform.openai.com/docs/api-reference/chat
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value chat(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<std::string> req{ "model", "messages" };
            for (auto& iter : req)
            {
                if (!request[iter])
                {
                    throw Php::Exception(getErrorReqMessage(iter));
                }
            }

            std::vector<std::string> n{ "max_tokens", "temperature", "top_p", "n", "logprobs", "presence_penalty", "frequency_penalty" };
            std::vector<std::string> b{ "stream" };
            std::vector<std::string> a{ "messages" };

            Php::Value r_message = request["messages"];
            request["messages"] = nullptr;

            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                    else {
                        j[iter.first] = nullptr;
                    }

                }
                else if (in_array(iter.first, b))
                {
                    j[iter.first] = iter.second == "true" ? true : false;
                }
                else if (in_array(iter.first, a))
                {
                    bool is_message_array = (r_message[0]["content"]) ? true : false;
                    json j2;

                    if (is_message_array)
                    {
                        for (auto& e1 : r_message)
                        {
                            std::map<std::string, std::string> el = e1.second;

                            for (auto& e2 : el)
                            {
                                j2[e2.first] = e2.second;
                            }                            

                            j[iter.first].push_back(j2);
                        }
                    }
                    else {
                        std::map<std::string, std::string> el = r_message;

                        for (auto& e : el)
                        {
                            j2[e.first] = e.second;
                        }

                        j[iter.first].push_back(j2);
                    }
                }
                else {
                    j[iter.first] = iter.second;
                }
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_CHAT);
#else
            auto c = openai_instance.chat.create(j);
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Edits
     * https://platform.openai.com/docs/api-reference/edits
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value edit(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<std::string> req{ "model", "instruction" };
            for (auto& iter : req)
            {
                if (!request[iter])
                {
                    throw Php::Exception(getErrorReqMessage(iter));
                }
            }

            std::vector<std::string> n{ "n", "temperature", "top_p" };
            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                    else {
                        j[iter.first] = nullptr;
                    }

                }
                else {
                    j[iter.first] = iter.second;
                }
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_EDIT);
#else
            auto c = openai_instance.edit.create(j);
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Images
     * https://platform.openai.com/docs/api-reference/images
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value image(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int create = (short int)params[1];
            short int response = (count(params) > 2) ? (short int)params[2] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<short int> v{ PHP_OPENAI_IMAGE_CREATE, PHP_OPENAI_IMAGE_EDIT, PHP_OPENAI_IMAGE_VARIATION };
            if (!(find(v.begin(), v.end(), create) != v.end()))
            {
                throw Php::Exception(getErrorInvMessage("create"));
            }

            std::vector<std::string> req;

            if (create == PHP_OPENAI_IMAGE_CREATE) 
            {
                req.push_back("prompt");
            }
            else if (create == PHP_OPENAI_IMAGE_EDIT) 
            {
                req.push_back("prompt");
                req.push_back("image");
            }
            else 
            {
                req.push_back("image");
            }

            for (auto& iter : req)
            {
                if (!request[iter])
                {
                    throw Php::Exception(getErrorReqMessage(iter));
                }
            }

            std::vector<std::string> n{ "n" };
            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                }
                else {
                    j[iter.first] = iter.second;
                }
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_IMAGE);
#else
            json c;

            switch (create)
            {
            case PHP_OPENAI_IMAGE_CREATE:
                c = openai_instance.image.create(j);
                break;
            case PHP_OPENAI_IMAGE_EDIT:
                c = openai_instance.image.edit(j);
                break;
            case PHP_OPENAI_IMAGE_VARIATION:
                c = openai_instance.image.variation(j);
                break;
            }
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Embeddings
     * https://platform.openai.com/docs/api-reference/embeddings
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value embedding(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<std::string> req{ "model", "input" };
            for (auto& iter : req)
            {
                if (!request[iter])
                {
                    throw Php::Exception(getErrorReqMessage(iter));
                }
            }

            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                j[iter.first] = iter.second;
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_EMBEDING);
#else
            auto c = openai_instance.embedding.create(j);
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Audio
     * https://platform.openai.com/docs/api-reference/audio
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value audio(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int create = (short int)params[1];
            short int response = (count(params) > 2) ? (short int)params[2] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<short int> v{ PHP_OPENAI_AUDIO_TRANSCRIPTION, PHP_OPENAI_AUDIO_TRANSLATION };
            if (!(find(v.begin(), v.end(), create) != v.end()))
            {
                throw Php::Exception(getErrorInvMessage("create"));
            }

            std::vector<std::string> req{ "model", "file" };
            for (auto& iter : req)
            {
                if (!request[iter])
                {
                    throw Php::Exception(getErrorReqMessage(iter));
                }
            }

            std::vector<std::string> n{ "temperature" };
            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                }
                else {
                    j[iter.first] = iter.second;
                }
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_AUDIO);
#else
            json c;

            switch (create)
            {
            case PHP_OPENAI_AUDIO_TRANSCRIPTION:
                c = openai_instance.audio.transcribe(j);
                break;
            case PHP_OPENAI_AUDIO_TRANSLATION:
                c = openai_instance.audio.translate(j);
                break;
            }
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Files
     * https://platform.openai.com/docs/api-reference/files
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value file(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int command = (short int)params[1];
            short int response = (count(params) > 2) ? (short int)params[2] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<short int> v{ PHP_OPENAI_FILE_LIST, PHP_OPENAI_FILE_UPLOAD, PHP_OPENAI_FILE_DELETE, PHP_OPENAI_FILE_RETRIEVE, PHP_OPENAI_FILE_CONTENT };
            if (!(find(v.begin(), v.end(), command) != v.end()))
            {
                throw Php::Exception(getErrorInvMessage("command"));
            }

            if (command == PHP_OPENAI_FILE_UPLOAD)
            {
                std::vector<std::string> req{ "file", "purpose" };
                for (auto& iter : req)
                {
                    if (!request[iter])
                    {
                        throw Php::Exception(getErrorReqMessage(iter));
                    }
                }
            }
            else if (command == PHP_OPENAI_FILE_DELETE || command == PHP_OPENAI_FILE_RETRIEVE || command == PHP_OPENAI_FILE_CONTENT)
            {
                if (!request["file_id"])
                {
                    throw Php::Exception(getErrorReqMessage("file_id"));
                }
            }

            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                j[iter.first] = iter.second;
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_FILE_LIST);
#else
            json c;

            switch (command)
            {
            case PHP_OPENAI_FILE_LIST:
                c = openai_instance.file.list();
                break;
            case PHP_OPENAI_FILE_UPLOAD:
                c = openai_instance.file.upload(j);
                break;
            case PHP_OPENAI_FILE_DELETE:
                c = openai_instance.file.del(request["file_id"]);
                break;
            case PHP_OPENAI_FILE_RETRIEVE:
                c = openai_instance.file.retrieve(request["file_id"]);
                break;
            case PHP_OPENAI_FILE_CONTENT:
                c = openai_instance.file.content(request["file_id"]);
                break;
        }
#endif
#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Fine-tunes
     * https://platform.openai.com/docs/api-reference/fine-tunes
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value fineTune(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int command = (short int)params[1];
            short int response = (count(params) > 2) ? (short int)params[2] : PHP_OPENAI_RESPONSE_ARRAY;

            std::vector<short int> v{ PHP_OPENAI_FINETUNE_CREATE, PHP_OPENAI_FINETUNE_LIST, PHP_OPENAI_FINETUNE_RETRIEVE, PHP_OPENAI_FINETUNE_CANCEL, PHP_OPENAI_FINETUNE_EVENTS, PHP_OPENAI_FINETUNE_DELETE };
            if (!(find(v.begin(), v.end(), command) != v.end()))
            {
                throw Php::Exception(getErrorInvMessage("command"));
            }

            if (command == PHP_OPENAI_FINETUNE_CREATE)
            {
                if (!request["training_file"])
                {
                    throw Php::Exception(getErrorReqMessage("training_file"));
                }
            }
            else if (command == PHP_OPENAI_FINETUNE_RETRIEVE || command == PHP_OPENAI_FINETUNE_CANCEL || command == PHP_OPENAI_FINETUNE_EVENTS)
            {
                if (!request["fine_tune_id"])
                {
                    throw Php::Exception(getErrorReqMessage("fine_tune_id"));
                }
            }
            else if (command == PHP_OPENAI_FINETUNE_DELETE)
            {
                if (!request["model"])
                {
                    throw Php::Exception(getErrorReqMessage("model"));
                }
            }

            std::vector<std::string> n{ "n_epochs", "batch_size", "learning_rate_multiplier", "prompt_loss_weight", "classification_n_classes" };
            std::vector<std::string> b{ "compute_classification_metrics" };
            std::vector<std::string> a{ "classification_betas" };
            std::map<std::string, std::string> r = request;
            json j;

            Php::Value r_classification_betas = nullptr;
            if (request["classification_betas"])
            {
                Php::Value r_classification_betas = request["classification_betas"];
            }

            for (auto& iter : r)
            {
                if (in_array(iter.first, n))
                {
                    if (is_integer(iter.second))
                    {
                        j[iter.first] = std::stoi(iter.second);
                    }
                    else if (is_float(iter.second))
                    {
                        j[iter.first] = std::stof(iter.second);
                    }
                    else {
                        j[iter.first] = nullptr;
                    }

                }
                else if (in_array(iter.first, b))
                {
                    j[iter.first] = iter.second == "true" ? true : false;
                }
                else if (in_array(iter.first, a))
                {
                    std::map<std::string, std::string> el = r_classification_betas;

                    for (auto& e : el)
                    {
                        j[iter.first][e.first] = e.second;
                    }

                }
                else {
                    j[iter.first] = iter.second;
                }
            }
#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_FINETUNE_CREATE);
#else
            json c;

            switch (command)
            {
            case PHP_OPENAI_FINETUNE_CREATE:
                c = openai_instance.fine_tune.create(j);
                break;
            case PHP_OPENAI_FINETUNE_LIST:
                c = openai_instance.fine_tune.list();
                break;
            case PHP_OPENAI_FINETUNE_RETRIEVE:
                c = openai_instance.fine_tune.retrieve(j["fine_tune_id"]);
                break;
            case PHP_OPENAI_FINETUNE_CANCEL:
                c = openai_instance.fine_tune.cancel(j["fine_tune_id"]);
                break;
            case PHP_OPENAI_FINETUNE_EVENTS:
                c = openai_instance.fine_tune.events(j["fine_tune_id"]);
                break;
            case PHP_OPENAI_FINETUNE_DELETE:
                c = openai_instance.fine_tune.del(j["model"]);
                break;
            }
#endif

#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }

    /**
     * OpenAI: Moderations
     * https://platform.openai.com/docs/api-reference/moderations
     * 
     * @param   params  request as an array
     * @return          response as an array or text in JSON format
     */
    Php::Value moderation(Php::Parameters& params)
    {
        if (!params.empty())
        {
            Php::Array request = params[0];
            short int response = (count(params) > 1) ? (short int)params[1] : PHP_OPENAI_RESPONSE_ARRAY;

            if (!request["input"])
            {
                throw Php::Exception(getErrorReqMessage("input"));
            }

            std::map<std::string, std::string> r = request;
            json j;

            for (auto& iter : r)
            {
                j[iter.first] = iter.second;
            }

#if PHP_OPENAI_TEST
            json c = json::parse(OPENAI_EXAMPLE_MODERATION);
#else
            auto c = openai_instance.moderation.create(j);
#endif

#ifdef PHP_WIN32
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::call("constant", "OPENAI_LOG_ENABLE") == 1))
#else
            if (log_enable || (Php::defined("OPENAI_LOG_ENABLE") && Php::constant("OPENAI_LOG_ENABLE") == 1))
#endif
            {
                saveLog(j.dump(2), c.dump(2));
            }

            if (response == PHP_OPENAI_RESPONSE_JSON)
            {
                return c.dump(2);
            }
            else {
                return to_array(c);
            }
        }

        return false;
    }
};

/**
 * Callback for phpinfo()
 */
INFO_FUNC(phpinfo)
{
    bool asText = static_cast<bool>((std::string)Php::sapi_name() == "cli");

    std::string phpInfo = "";
    std::unordered_map<std::string, std::string> infoMap;

    infoMap["Version"] = PHP_OPENAI_VERSION;

    // php_info_print_table_start()
    phpInfo = ((!asText) ? "<table>\n" : "\n");

    // php_info_print_table_header()
    for (auto& iter : infoMap) 
    {
        if (!asText) 
        {
            phpInfo += "<tr>";
            phpInfo += "<td class=\"e\">" + iter.first + "</td>";
            phpInfo += "<td class=\"v\">" + iter.second + "</td>";
            phpInfo += "</tr>\n";
        }
        else {
            phpInfo = phpInfo + iter.first + " => " + iter.second + "\n";
        }
    }

    phpInfo += ((!asText) ? "<table>\n<tr class=\"v\"><td>" : "\n");
    phpInfo += "OpenAI PHP extension library implementation by Mariusz Duka " + (std::string)((!asText) ? "&copy; 2023.<br />" : "(c) 2023\n");
    phpInfo += "For more information about this extension, please visit " + (std::string)PHP_OPENAI_WEBPAGE + (std::string)((!asText) ? ".<br/>" : ".\n");
    phpInfo += "GitHub repository at " + (std::string)PHP_OPENAI_GITHUB + (std::string)((!asText) ? ".<br/>" : ".\n");

    // php_info_print_table_end()
    phpInfo += ((!asText) ? "</td></tr></table>\n" : "\n");

    // ini variables
    int iniCount = (int)extension->iniVariables();
    
    if (iniCount > 0)
    {
        if (!asText)
        {
            phpInfo += "<table><tr class=\"h\"><th>Directive</th><th>Local Value</th><th>Master Value</th></tr>\n";
        }
        else {
            phpInfo += "\nDirective => Local Value => Master Value\n";
        }

        extension->iniVariables([asText, &phpInfo](Php::Ini& ini)
        {
            std::string name = ini.name();
            Php::IniValue master = Php::ini_get_orig(name.c_str());
            Php::IniValue local = Php::ini_get(name.c_str());
            std::string masterVal = master;
            std::string localVal = local;
            masterVal = (masterVal.empty()) ? "no value" : masterVal;
            masterVal = (masterVal == "no value" && !asText) ? "<i>" + masterVal + "</i>" : masterVal;
            localVal = (localVal.empty()) ? "no value" : localVal;
            localVal = (localVal == "no value" && !asText) ? "<i>" + localVal + "</i>" : localVal;

            if (!asText)
            {
                phpInfo += "<tr>";
                phpInfo += "<td class=\"e\">" + name + "</td><td class=\"v\">" + localVal + "</td><td class=\"v\">" + masterVal + "</td>";
                phpInfo += "</tr>\n";
            }
            else
            {
                phpInfo += name + " => " + localVal + " => " + masterVal + "\n";
            }
        });

        if (!asText)
        {
            phpInfo += "</table>\n";
        }
    }

    Php::out << phpInfo << std::flush;

    /*    
    Version => 1.0.0

    OpenAI PHP extension library implementation by Mariusz Duka (c) 2023
    For more information about this extension, please visit https://openai-php.com.
    GitHub repository at https://github.com/mariuszduka/openai-php.

    Directive => Local Value => Master Value
    openai.log_enable => 0 => 0
    openai.log_filepath => ./openai.log => ./openai.log
    */
}

/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {

    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void* get_module()
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
        static Php::Extension openai("openai", PHP_OPENAI_VERSION);

        Php::Namespace OpenAINamespace("OpenAI");

        // description of the class so that PHP knows which methods are accessible
        Php::Class<OpenAI> OpenAI("OpenAI");

        // the OpenAI class has one public property
        OpenAI.property("PHP_OPENAI_VERSION", PHP_OPENAI_VERSION, Php::Const);
        OpenAI.property("PHP_OPENAI_RESPONSE_ARRAY", PHP_OPENAI_RESPONSE_ARRAY, Php::Const);
        OpenAI.property("PHP_OPENAI_RESPONSE_JSON", PHP_OPENAI_RESPONSE_JSON, Php::Const);

        OpenAI.property("PHP_OPENAI_IMAGE_CREATE", PHP_OPENAI_IMAGE_CREATE, Php::Const);
        OpenAI.property("PHP_OPENAI_IMAGE_EDIT", PHP_OPENAI_IMAGE_EDIT, Php::Const);
        OpenAI.property("PHP_OPENAI_IMAGE_VARIATION", PHP_OPENAI_IMAGE_VARIATION, Php::Const);

        OpenAI.property("PHP_OPENAI_AUDIO_TRANSCRIPTION", PHP_OPENAI_AUDIO_TRANSCRIPTION, Php::Const);
        OpenAI.property("PHP_OPENAI_AUDIO_TRANSLATION", PHP_OPENAI_AUDIO_TRANSLATION, Php::Const);

        OpenAI.property("PHP_OPENAI_FILE_LIST", PHP_OPENAI_FILE_LIST, Php::Const);
        OpenAI.property("PHP_OPENAI_FILE_UPLOAD", PHP_OPENAI_FILE_UPLOAD, Php::Const);
        OpenAI.property("PHP_OPENAI_FILE_DELETE", PHP_OPENAI_FILE_DELETE, Php::Const);
        OpenAI.property("PHP_OPENAI_FILE_RETRIEVE", PHP_OPENAI_FILE_RETRIEVE, Php::Const);
        OpenAI.property("PHP_OPENAI_FILE_CONTENT", PHP_OPENAI_FILE_CONTENT, Php::Const);

        OpenAI.property("PHP_OPENAI_FINETUNE_CREATE", PHP_OPENAI_FINETUNE_CREATE, Php::Const);
        OpenAI.property("PHP_OPENAI_FINETUNE_LIST", PHP_OPENAI_FINETUNE_LIST, Php::Const);
        OpenAI.property("PHP_OPENAI_FINETUNE_RETRIEVE", PHP_OPENAI_FINETUNE_RETRIEVE, Php::Const);
        OpenAI.property("PHP_OPENAI_FINETUNE_CANCEL", PHP_OPENAI_FINETUNE_CANCEL, Php::Const);
        OpenAI.property("PHP_OPENAI_FINETUNE_EVENTS", PHP_OPENAI_FINETUNE_EVENTS, Php::Const);
        OpenAI.property("PHP_OPENAI_FINETUNE_DELETE", PHP_OPENAI_FINETUNE_DELETE, Php::Const);

        OpenAI.method<&OpenAI::__construct>("__construct", 
        {
            Php::ByVal("api_key", Php::Type::String, true),
            Php::ByVal("org_id", Php::Type::String, false)
        });

        OpenAI.method<&OpenAI::models>("models", Php::Public | Php::Final, 
        {
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::model>("model", Php::Public | Php::Final, 
        {
            Php::ByVal("model", Php::Type::String, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::completion>("completion", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::chat>("chat", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::edit>("edit", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::image>("image", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("create", Php::Type::Numeric, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::embedding>("embedding", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::audio>("audio", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("create", Php::Type::Numeric, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::file>("file", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("command", Php::Type::Numeric, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::fineTune>("fineTune", Php::Public | Php::Final,
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("command", Php::Type::Numeric, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        OpenAI.method<&OpenAI::moderation>("moderation", Php::Public | Php::Final, 
        {
            Php::ByVal("request", Php::Type::Array, true),
            Php::ByVal("response", Php::Type::Numeric, false)
        });

        // add the class to the namespace
        OpenAINamespace.add(std::move(OpenAI));

        // add the namespace to the extension
        openai.add(std::move(OpenAINamespace));

        // tell the PHP engine that the php.ini variables are usable
        openai.add(Php::Ini("openai.log_enable", PHP_OPENAI_LOG_ENABLE));
        openai.add(Php::Ini("openai.log_filepath", PHP_OPENAI_LOG_FILE));

        openai.onStartup([]() {
            
        });
        openai.onRequest([]() {

        });
        openai.onIdle([]() {

        });
        openai.onShutdown([]() {

        });

        // register callback for phpinfo()
        Php::Extension* extPtr = &openai;
        
        // and install it
        openai.onInfo([extPtr]() {
            CALL_INFO_FUNC(phpinfo, extPtr);
        });

        // return a pointer to the extension object
        return openai;
    }
}
<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create chat completion */
    $chat = $OpenAI->chat(request: [
        "model"    => "gpt-3.5-turbo",
        "messages" => [
            [
                "role" => "system", 
                "content" => "You know everything about Silesia."
            ],
            [
                "role" => "user", 
                "content" => "What is the largest city in Silesia?"
            ],
            [
                "role" => "assistant", 
                "content" => "The largest city in Silesia is Katowice."
            ],
            [
                "role" => "user", 
                "content" => "How many people live there?"
            ]
        ]
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($chat);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
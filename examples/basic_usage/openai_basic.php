<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    $response = $OpenAI->chat(request:
    [
        "model"    => "gpt-3.5-turbo",
        "messages" => 
        [
            "role"    => "user",
            "content" => "Do you know Elon Musk?"
        ]
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($response);
}
catch (Exception $e)
{
    echo("Hmm, something went wrong :(").PHP_EOL;
    echo $e->getMessage();
}

unset($OpenAI);
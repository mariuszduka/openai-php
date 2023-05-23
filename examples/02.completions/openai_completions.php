<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create completion */
    $completion = $OpenAI->completion(request: 
    [
        "model"       => "text-davinci-003",
        "prompt"      => "Silesia is famous for its",
        "max_tokens"  => 12,
        "temperature" => 1,
        "top_p"       => 1,
        "n"           => 2,
        "stream"      => false,
        "logprobs"    => null,
        "stop"        => "\n"
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($completion);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
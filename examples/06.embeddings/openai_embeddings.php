<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create embedding */
    $embedding = $OpenAI->embedding(request: [
        "model"     => "text-embedding-ada-002",
        "input"     => "The weather was great and the coach..."
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($embedding);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
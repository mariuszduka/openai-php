<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create moderation */
    $moderation = $OpenAI->moderation(request: [
        "input" => "I'm going to kill these sons of bitches.",
        "model" => "text-moderation-stable"
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($moderation);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
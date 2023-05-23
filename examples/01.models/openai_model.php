<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Retrieve model */
    $model = $OpenAI->model(
        model: "gpt-3.5-turbo",
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print "Basic information about the gpt-3.5-turbo model:".PHP_EOL;
    print_r($model);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
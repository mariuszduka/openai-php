<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* List models */
    $models = $OpenAI->models(
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print "List of currently available models:".PHP_EOL;
    foreach($models["data"] as $model)
    {
        print "Model ID: ".$model["id"].PHP_EOL;
    }
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
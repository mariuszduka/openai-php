<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* List files */
    $files = $OpenAI->file(request: [],
        command: OpenAI::PHP_OPENAI_FILE_LIST,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($files);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
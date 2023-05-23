<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Retrieve file */
    $file_info = $OpenAI->file(request: [
        "file_id" => "FILE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FILE_RETRIEVE,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($file_info);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
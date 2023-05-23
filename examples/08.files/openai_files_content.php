<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Retrieve file content */
    $file_content = $OpenAI->file(request: [
        "file_id" => "FILE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FILE_CONTENT,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($file_content);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
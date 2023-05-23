<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Upload file */
    $file_upload = $OpenAI->file(request: [
        "file"    => "fine-tune-01.jsonl",
        "purpose" => "fine-tune"
    ],
        command: OpenAI::PHP_OPENAI_FILE_UPLOAD,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($file_upload);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
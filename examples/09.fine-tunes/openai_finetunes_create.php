<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create fine-tune */
    $finetune_create = $OpenAI->fineTune(request: [
        "training_file" => "FILE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FINETUNE_CREATE, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_create);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
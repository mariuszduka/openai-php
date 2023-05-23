<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Retrieve fine-tune */
    $finetune_info = $OpenAI->fineTune(request: [
        "fine_tune_id" => "FINE_TUNE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FINETUNE_RETRIEVE, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_info);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
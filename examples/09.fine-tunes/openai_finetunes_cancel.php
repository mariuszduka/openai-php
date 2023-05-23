<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Cancel fine-tune */
    $finetune_cancel = $OpenAI->fineTune(request: [
        "fine_tune_id" => "FINE_TUNE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FINETUNE_CANCEL, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_cancel);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
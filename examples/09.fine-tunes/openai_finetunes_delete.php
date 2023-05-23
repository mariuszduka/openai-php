<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Delete fine-tune model */
    $finetune_delete = $OpenAI->fineTune(request: [
        "model" => "MODEL_ID"
    ],
        command: OpenAI::PHP_OPENAI_FINETUNE_DELETE, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_delete);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
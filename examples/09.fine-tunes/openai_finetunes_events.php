<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* List fine-tune events */
    $finetune_events = $OpenAI->fineTune(request: [
        "fine_tune_id" => "FINE_TUNE_ID"
    ],
        command: OpenAI::PHP_OPENAI_FINETUNE_EVENTS,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_events);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
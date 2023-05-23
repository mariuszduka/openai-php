<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* List fine-tunes */
    $finetune_list = $OpenAI->fineTune(request: [],
        command: OpenAI::PHP_OPENAI_FINETUNE_LIST, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($finetune_list);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
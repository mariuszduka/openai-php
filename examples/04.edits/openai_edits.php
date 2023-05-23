<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create edit */
    $edit = $OpenAI->edit(request: [
        "model"       => "text-davinci-edit-001",
        "input"       => "What's the temperture outside?",
        "instruction" => "Fix the spelling mistakes"
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($edit);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
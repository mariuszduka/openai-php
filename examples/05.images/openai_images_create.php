<?php

namespace OpenAI;
use Exception;

try
{
    $OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

    /* Create image */
    $image = $OpenAI->image(request: [
        "prompt"    => "Vase on the kitchen table.",
        "n"         => 2,
        "size"      => "512x512"
    ],
        create: OpenAI::PHP_OPENAI_IMAGE_CREATE,
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($image);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
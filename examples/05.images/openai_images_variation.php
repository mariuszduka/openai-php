<?php

namespace OpenAI;
use Exception;

try
{
    $OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

    /* Create image variation */
    $image = $OpenAI->image(request: [
        "image"     => "vase_and_phone_on_table.png",
        "n"         => 2,
        "size"      => "512x512"
    ],
        create: OpenAI::PHP_OPENAI_IMAGE_VARIATION, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($image);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
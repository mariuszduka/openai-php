<?php

namespace OpenAI;
use Exception;

try
{
    $OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

    /* Create image edit */
    $image = $OpenAI->image(request: [
        "image"     => "vase_on_table.png",
        "mask"      => "vase_on_table_mask.png",
        "prompt"    => "A vase and a mobile phone on the kitchen table.",
        "n"         => 2,
        "size"      => "512x512"
    ], 
        create: OpenAI::PHP_OPENAI_IMAGE_EDIT, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($image);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create translation */
    $translation = $OpenAI->audio(request: [
        "file"      => "audio_fr.mp3",
        "model"     => "whisper-1"
    ],
        create: OpenAI::PHP_OPENAI_AUDIO_TRANSLATION, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($translation);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
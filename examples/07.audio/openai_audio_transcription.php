<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    /* Create transcription */
    $transcription = $OpenAI->audio(request: [
        "file"      => "audio_en.mp3",
        "model"     => "whisper-1"
    ],
        create: OpenAI::PHP_OPENAI_AUDIO_TRANSCRIPTION, 
        response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY
    );

    print_r($transcription);
} 
catch(Exception $e)
{
    echo $e->getMessage();
}

unset($OpenAI);
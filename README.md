# OpenAI PHP extension library

[![](https://badgen.net/badge/C++/11/blue)](https://isocpp.org/) [![](https://badgen.net/badge/PHP/7.4/blue)](https://www.php.net/) [![](https://badgen.net/badge/PHP/8.0/blue)](https://www.php.net/releases/8.0/) [![](https://badgen.net/badge/PHP/8.1/blue)](https://www.php.net/releases/8.1/) [![](https://badgen.net/badge/PHP/8.2/blue)](https://www.php.net/releases/8.2/) [![](https://badgen.net/badge/license/MIT/green)](https://github.com/mariuszduka/openai-php/blob/main/LICENSE) [![](https://badgen.net/badge/version/1.0.0/orange)](https://github.com/mariuszduka/openai-php/releases)

The [OpenAI PHP](https://openai-php.com) is an extension that allows [PHP](https://www.php.net/) developers to integrate the powerful capabilities of [OpenAI](https://openai.com/) into their applications. Activating the extension on the web server will allow users to quickly access the services offered by OpenAI without additional libraries in PHP.

> OpenAI PHP uses the PHP-CPP, OpenAI C++ and JSON for Modern C++ libraries.

## Download and install

Download the latest [release](https://github.com/mariuszduka/openai-php/releases) of the library for Linux (.so files) and Windows (.dll files).<br>Supported PHP versions: `7.4`, `8.0`, `8.1` and `8.2`.

You can also download the libraries from the website [openai-php.com](https://openai-php.com).

### Linux

> On Linux systems, the `libcurl` library must be installed.

Before installation, check the installed version of PHP (NTS - non-tread safe, ZTS - thread safe) on your system with the command:

```bash
php -v
```

Unzip the archive and enter the root directory, for example:

```bash
tar xf openai-php-binary-linux.tar.gz
cd openai-php-binary-linux
```

Now, you can use the `PHP`, `MODE` and `ZTS` parameters in the `make` command. `PHP` means the PHP version (7.4, 8.0, 8.1, 8.2), `MODE` means operating modes (cli, fpm or apache2) and `ZTS` means thread safe (`enable`) or non-thread safe (`disable`) version.

The syntax for `make` is:

```bash
make PHP=7.4,8.0,8.1,8.2 MODE=cli,fpm,apache2 ZTS=enable,disable
```

For example, to install the extension for PHP 8.2, execute the command:

* for PHP 8.2 ZTS (thread safe), CLI mode:

```bash
make PHP=8.2 MODE=cli ZTS=enable
```

* for PHP 8.2 NTS (non-thread safe), FPM mode:

```bash
make PHP=8.2 MODE=fpm ZTS=disable
```

By default, if `MODE` parameter is not defined, the extension is installed for the `PHP CLI`.

Done! You may need to reload the webserver.

To uninstall the extension, execute the command:

```bash
make PHP=7.4,8.0,8.1,8.2 MODE=cli,fpm,apache2 uninstall
```

### Windows

Download the latest release of binaries for Windows. Find the directory where PHP is installed and then upload the extension to the `ext` directory. For example, if you have installed the XAMPP on your `C` drive, upload the extension to the `C:\xampp\php\ext` directory.

Activate the extension by adding the following line to the `php.ini` file:

```ini
extension=openai.dll
```

### After installation and configuration

Use the `phpinfo()` function to get detailed information about active extensions in PHP.

## Basic usage

1. Generate an `API key` on the website [OpenAI](https://platform.openai.com/account/api-keys).
2. Run the [following code](examples/basic_usage/openai_basic.php) in a console or browser. Done!

```php
<?php

namespace OpenAI;
use Exception;

$OpenAI = new OpenAI(api_key: "YOUR_API_KEY");

try
{
    $response = $OpenAI->chat(request:
    [
        "model"    => "gpt-3.5-turbo",
        "messages" => 
        [
            "role"    => "user",
            "content" => "Do you know Elon Musk?"
        ]
    ], response: OpenAI::PHP_OPENAI_RESPONSE_ARRAY);

    print_r($response);
}
catch (Exception $e)
{
    echo("Hmm, something went wrong :(").PHP_EOL;
    echo $e->getMessage();
}

unset($OpenAI);
```

## Advanced usage

### Enable or disable the saving of logs to a file

The OpenAI PHP extension allows you to save all requests sent to the OpenAI system and all responses to log files. You can activate this function globally in the `php.ini` file:

```ini
openai.log_enable = 1
openai.log_filepath = "./openai.log"
```

Enabling this feature may increase server load, so the optimal option is to activate logging by the user. An example of activating this function in [PHP code](examples/advanced_usage/openai_logs.php) by the user might be:

```php
define("OPENAI_LOG_ENABLE", 1);
define("OPENAI_LOG_FILEPATH", "./openai.log");
```

### The format of the returned data

The default `response` parameter value is `OpenAI::PHP_OPENAI_RESPONSE_ARRAY`. This means that all functions return data as arrays by default. To return data in JSON format, use the `OpenAI::PHP_OPENAI_RESPONSE_JSON` constants.

```php
$models = $OpenAI->models(
    response: OpenAI::PHP_OPENAI_RESPONSE_JSON
);
```

### PHP 7

In PHP 7 (and earlier versions), function parameters are declared with a dollar sign followed by the parameter name, like this:

```php
<?php
namespace OpenAI;

$OpenAI = new OpenAI($api_key = "YOUR_API_KEY");

$models = $OpenAI->models(
    $response = OpenAI::PHP_OPENAI_RESPONSE_ARRAY
);
```

## Features

#### OpenAI API

| API reference | Method | Example file |
| --- | --- | --- |
| API models | [List models](https://platform.openai.com/docs/api-reference/models/list) | [openai_models.php](examples/01.models/openai_models.php) |
| API models | [Retrieve model](https://platform.openai.com/docs/api-reference/models/retrieve) | [openai_model.php](examples/01.models/openai_model.php) |
| API completions | [Create completion](https://platform.openai.com/docs/api-reference/completions/create) | [openai_completions.php](examples/02.completions/openai_completions.php) |
| API chat | [Create chat completion](https://platform.openai.com/docs/api-reference/chat/create) | [openai_chat.php](examples/03.chat/openai_chat.php) |
| API edits | [Create completion](https://platform.openai.com/docs/api-reference/completions/create) | [openai_edits.php](examples/04.edits/openai_edits.php) |
| API images | [Create image](https://platform.openai.com/docs/api-reference/images) | [openai_images_create.php](examples/05.images/openai_images_create.php) |
| API images | [Create image edit](https://platform.openai.com/docs/api-reference/images/create-edit) | [openai_images_edit.php](examples/05.images/openai_images_edit.php) |
| API images | [Create image variation](https://platform.openai.com/docs/api-reference/images/create-variation) | [openai_images_variation.php](examples/05.images/openai_images_variation.php) |
| API embeddings | [Create embeddings](https://platform.openai.com/docs/api-reference/embeddings/create) | [openai_embeddings.php](examples/06.embeddings/openai_embeddings.php) |
| API audio | [Create transcription](https://platform.openai.com/docs/api-reference/audio/create) | [openai_audio_transcription.php](examples/07.audio/openai_audio_transcription.php) |
| API audio | [Create translation](https://platform.openai.com/docs/api-reference/audio/create) | [openai_audio_translation.php](examples/07.audio/openai_audio_translation.php) |
| API files | [List file](https://platform.openai.com/docs/api-reference/files/list) | [openai_files_list.php](examples/08.files/openai_files_list.php) |
| API files | [Upload file](https://platform.openai.com/docs/api-reference/files/upload) | [openai_files_upload.php](examples/08.files/openai_files_upload.php) |
| API files | [Delete file](https://platform.openai.com/docs/api-reference/files/delete) | [openai_files_delete.php](examples/08.files/openai_files_delete.php) |
| API files | [Retrieve file](https://platform.openai.com/docs/api-reference/files/retrieve) | [openai_files_retrieve.php](examples/08.files/openai_files_retrieve.php) |
| API files | [Retrieve file content](https://platform.openai.com/docs/api-reference/files/retrieve-content) | [openai_files_content.php](examples/08.files/openai_files_content.php) |
| API fine-tunes | [Create fine-tune](https://platform.openai.com/docs/api-reference/fine-tunes/create) | [openai_finetunes_create.php](examples/09.fine-tunes/openai_finetunes_create.php) |
| API fine-tunes | [List fine-tune](https://platform.openai.com/docs/api-reference/fine-tunes/list) | [openai_finetunes_list.php](examples/09.fine-tunes/openai_finetunes_list.php) |
| API fine-tunes | [Retrieve fine-tune](https://platform.openai.com/docs/api-reference/fine-tunes/retrieve) | [openai_finetunes_retrieve.php](examples/09.fine-tunes/openai_finetunes_retrieve.php) |
| API fine-tunes | [Cancel fine-tune](https://platform.openai.com/docs/api-reference/fine-tunes/cancel) | [openai_finetunes_cancel.php](examples/09.fine-tunes/openai_finetunes_cancel.php) |
| API fine-tunes | [List fine-tune events](https://platform.openai.com/docs/api-reference/fine-tunes/events) | [openai_finetunes_events.php](examples/09.fine-tunes/openai_finetunes_events.php) |
| API fine-tunes | [Delete fine-tune model](https://platform.openai.com/docs/api-reference/fine-tunes/delete-model) | [openai_finetunes_delete.php](examples/09.fine-tunes/openai_finetunes_delete.php) |
| API moderation | [Create moderation](https://platform.openai.com/docs/api-reference/moderations/create) | [openai_moderations.php](examples/10.moderations/openai_moderations.php) |

#### Sample results

| Create image | Create image edit | Create image variation |
| --- | --- | --- |
| [![](examples/05.images/jpg/vase_on_table.jpg)](examples/05.images/jpg/vase_on_table.jpg) | [![](examples/05.images/jpg/vase_and_phone_on_table.jpg)](examples/05.images/jpg/vase_and_phone_on_table.jpg) | [![](examples/05.images/jpg/vase_and_phone_on_table_variation_1.jpg)](examples/05.images/jpg/vase_and_phone_on_table_variation_1.jpg) |

## Installation from source code

#### Requirements

* `C++11`/`C++14`/`C++17`/`C++20` compatible compiler
* [PHP-CPP library](https://github.com/CopernicaMarketingSoftware/PHP-CPP) or [PHP-CPP library for PHP 8](https://github.com/fast-debug/PHP-CPP)
* [OpenAI C++ library](https://github.com/olrea/openai-cpp)
* [JSON for Modern C++ library](https://github.com/nlohmann/json)
* [libcurl](https://curl.se/libcurl/)

The following commands have been tested on Ubuntu 22.04.

> If you are using Windows 11, you can use WSL to install Ubuntu.

1. Log in as an administrator and update all libraries.

```bash
sudo -i
apt update
apt upgrade
cd /opt
```

2. Install the development versions of `PHP` and `libcurl`.

```bash
apt install php8.1-dev
apt install libcurl4-gnutls-dev
```

3. Download `OpenAI PHP` repo:

```bash
git clone https://github.com/mariuszduka/openai-php
cd openai-php
```

4. Download and install `PHP-CPP` repo for PHP 8:

```bash
git clone https://github.com/fast-debug/PHP-CPP php-cpp
cd php-cpp
make
make install
make clean
cd ..
```

5. Download the `OpenAI C++` and `JSON for Modern C++` libraries:

```bash
wget -O libs/openai-cpp/openai.hpp https://raw.githubusercontent.com/olrea/openai-cpp/main/include/openai/openai.hpp
wget -O libs/openai-cpp/nlohmann/json.hpp https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp
```

6. Update `OpenAI C++` code for `OpenAI PHP` support:

```bash
patch libs/openai-cpp/openai.hpp libs/openai.hpp.patch
```

7. Build `OpenAI PHP` extension:

```bash
make
make MODE=cli install
```

The `MODE` parameter can be `cli`, `fpm` or `apache2`.

Done! You may need to reload the webserver.

8. Check that the `OpenAI PHP` extension is active in PHP:

```bash
php -m | grep openai
```

9. Run the [openai-basic.php](examples/basic_usage/openai_basic.php) file with: 
 
```bash
php openai-basic.php
```

## Known issues

Correct compilation of the PHP-CPP library causes the most problems, especially if we want to compile the library for Windows. Please check all [pull requests](https://github.com/CopernicaMarketingSoftware/PHP-CPP/pulls) as not all are accepted and you have to do it manually.

The optimal variant is to download the latest [release](https://github.com/mariuszduka/openai-php/releases) of the library for Linux and Windows. All you have to do is download and activate the extension in your PHP configuration.

## License

MIT License

Copyright (c) 2023 Mariusz Duka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
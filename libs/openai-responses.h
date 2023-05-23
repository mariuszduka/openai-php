/**
 * OpenAI PHP extension example response data
 */

#pragma once

auto OPENAI_EXAMPLE_MODELS = R"(
{
  "data": [
    {
      "id": "model-id-0",
      "object": "model",
      "owned_by": "organization-owner",
      "permission": []
    },
    {
      "id": "model-id-1",
      "object": "model",
      "owned_by": "organization-owner",
      "permission": []
    },
    {
      "id": "model-id-2",
      "object": "model",
      "owned_by": "openai",
      "permission": []
    }
  ],
  "object": "list"
}
)";

auto OPENAI_EXAMPLE_MODEL = R"(
{
  "created": 1669599635,
  "id": "text-davinci-003",
  "object": "model",
  "owned_by": "openai-internal",
  "parent": null,
  "permission": [
    {
      "allow_create_engine": false,
      "allow_fine_tuning": false,
      "allow_logprobs": true,
      "allow_sampling": true,
      "allow_search_indices": false,
      "allow_view": true,
      "created": 1681415378,
      "group": null,
      "id": "modelperm-XKctlRE4Wz1fIXFVcybPlWQE",
      "is_blocking": false,
      "object": "model_permission",
      "organization": "*"
    }
  ],
  "root": "text-davinci-003"
}
)";

auto OPENAI_EXAMPLE_COMPLETION = R"(
{
	"id": "cmpl-uqkvlQyYK7bGYrRHQ0eXlWi7",
	"object" : "text_completion",
	"created" : 1589478378,
	"model" : "text-davinci-003",
	"choices" : [
		{
			"text": "\n\nThis is indeed a test",
			"index" : 0,
			"logprobs" : null,
			"finish_reason" : "length"
		}
	],
	"usage": {
		"prompt_tokens": 5,
		"completion_tokens" : 7,
		"total_tokens" : 12
	}
}
)";

auto OPENAI_EXAMPLE_CHAT = R"(
{
  "id": "chatcmpl-123",
  "object": "chat.completion",
  "created": 1677652288,
  "choices": [{
    "index": 0,
    "message": {
      "role": "assistant",
      "content": "\n\nHello there, how may I assist you today?"
    },
    "finish_reason": "stop"
  }],
  "usage": {
    "prompt_tokens": 9,
    "completion_tokens": 12,
    "total_tokens": 21
  }
}
)";

auto OPENAI_EXAMPLE_EDIT = R"(
{
  "object": "edit",
  "created": 1589478378,
  "choices": [
    {
      "text": "What day of the week is it?",
      "index": 0
    }
  ],
  "usage": {
    "prompt_tokens": 25,
    "completion_tokens": 32,
    "total_tokens": 57
  }
}
)";

auto OPENAI_EXAMPLE_IMAGE = R"(
{
  "created": 1589478378,
  "data": [
    {
      "url": "https://..."
    },
    {
      "url": "https://..."
    }
  ]
}
)";

auto OPENAI_EXAMPLE_EMBEDING = R"(
{
  "object": "list",
  "data": [
    {
      "object": "embedding",
      "embedding": [
        0.0023064255,
        -0.009327292,
        -0.0028842222
      ],
      "index": 0
    }
  ],
  "model": "text-embedding-ada-002",
  "usage": {
    "prompt_tokens": 8,
    "total_tokens": 8
  }
}
)";

auto OPENAI_EXAMPLE_AUDIO = R"(
{
  "text": "Imagine the wildest idea that you've ever had, and you're curious about how it might scale to something that's a 100, a 1,000 times bigger. This is a place where you can get to do that."
}
)";

auto OPENAI_EXAMPLE_FILE_LIST = R"(
{
  "data": [
    {
      "id": "file-ccdDZrC3iZVNiQVeEA6Z66wf",
      "object": "file",
      "bytes": 175,
      "created_at": 1613677385,
      "filename": "train.jsonl",
      "purpose": "search"
    },
    {
      "id": "file-XjGxS3KTG0uNmNOK362iJua3",
      "object": "file",
      "bytes": 140,
      "created_at": 1613779121,
      "filename": "puppy.jsonl",
      "purpose": "search"
    }
  ],
  "object": "list"
}
)";

auto OPENAI_EXAMPLE_FILE_UPLOAD = R"(
{
  "id": "file-XjGxS3KTG0uNmNOK362iJua3",
  "object": "file",
  "bytes": 140,
  "created_at": 1613779121,
  "filename": "mydata.jsonl",
  "purpose": "fine-tune"
}
)";

auto OPENAI_EXAMPLE_FILE_DELETE = R"(
{
  "id": "file-XjGxS3KTG0uNmNOK362iJua3",
  "object": "file",
  "deleted": true
}
)";

auto OPENAI_EXAMPLE_FILE_RETRIEVE = R"(
{
  "id": "file-XjGxS3KTG0uNmNOK362iJua3",
  "object": "file",
  "bytes": 140,
  "created_at": 1613779657,
  "filename": "mydata.jsonl",
  "purpose": "fine-tune"
}
)";

auto OPENAI_EXAMPLE_FINETUNE_CREATE = R"(
{
  "id": "ft-AF1WoRqd3aJAHsqc9NY7iL8F",
  "object": "fine-tune",
  "model": "curie",
  "created_at": 1614807352,
  "events": [
    {
      "object": "fine-tune-event",
      "created_at": 1614807352,
      "level": "info",
      "message": "Job enqueued. Waiting for jobs ahead to complete. Queue number: 0."
    }
  ],
  "fine_tuned_model": null,
  "hyperparams": {
    "batch_size": 4,
    "learning_rate_multiplier": 0.1,
    "n_epochs": 4,
    "prompt_loss_weight": 0.1
  },
  "organization_id": "org-...",
  "result_files": [],
  "status": "pending",
  "validation_files": [],
  "training_files": [
    {
      "id": "file-XGinujblHPwGLSztz8cPS8XY",
      "object": "file",
      "bytes": 1547276,
      "created_at": 1610062281,
      "filename": "my-data-train.jsonl",
      "purpose": "fine-tune-train"
    }
  ],
  "updated_at": 1614807352
}
)";

auto OPENAI_EXAMPLE_FINETUNE_LIST = R"(
{
  "object": "list",
  "data": [
    {
      "id": "ft-AF1WoRqd3aJAHsqc9NY7iL8F",
      "object": "fine-tune",
      "model": "curie",
      "created_at": 1614807352,
      "fine_tuned_model": null,
      "hyperparams": { },
      "organization_id": "org-...",
      "result_files": [],
      "status": "pending",
      "validation_files": [],
      "training_files": [ { } ],
      "updated_at": 1614807352,
    }
  ]
}
)";

auto OPENAI_EXAMPLE_FINETUNE_RETRIEVE = R"(
{
  "id": "ft-AF1WoRqd3aJAHsqc9NY7iL8F",
  "object": "fine-tune",
  "model": "curie",
  "created_at": 1614807352,
  "events": [
    {
      "object": "fine-tune-event",
      "created_at": 1614807352,
      "level": "info",
      "message": "Job enqueued. Waiting for jobs ahead to complete. Queue number: 0."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807356,
      "level": "info",
      "message": "Job started."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807861,
      "level": "info",
      "message": "Uploaded snapshot: curie:ft-acmeco-2021-03-03-21-44-20."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807864,
      "level": "info",
      "message": "Uploaded result files: file-QQm6ZpqdNwAaVC3aSz5sWwLT."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807864,
      "level": "info",
      "message": "Job succeeded."
    }
  ],
  "fine_tuned_model": "curie:ft-acmeco-2021-03-03-21-44-20",
  "hyperparams": {
    "batch_size": 4,
    "learning_rate_multiplier": 0.1,
    "n_epochs": 4,
    "prompt_loss_weight": 0.1,
  },
  "organization_id": "org-...",
  "result_files": [
    {
      "id": "file-QQm6ZpqdNwAaVC3aSz5sWwLT",
      "object": "file",
      "bytes": 81509,
      "created_at": 1614807863,
      "filename": "compiled_results.csv",
      "purpose": "fine-tune-results"
    }
  ],
  "status": "succeeded",
  "validation_files": [],
  "training_files": [
    {
      "id": "file-XGinujblHPwGLSztz8cPS8XY",
      "object": "file",
      "bytes": 1547276,
      "created_at": 1610062281,
      "filename": "my-data-train.jsonl",
      "purpose": "fine-tune-train"
    }
  ],
  "updated_at": 1614807865,
}
)";

auto OPENAI_EXAMPLE_FINETUNE_CANCEL = R"(
{
  "id": "ft-xhrpBbvVUzYGo8oUO1FY4nI7",
  "object": "fine-tune",
  "model": "curie",
  "created_at": 1614807770,
  "events": [ { } ],
  "fine_tuned_model": null,
  "hyperparams": { },
  "organization_id": "org-...",
  "result_files": [],
  "status": "cancelled",
  "validation_files": [],
  "training_files": [
    {
      "id": "file-XGinujblHPwGLSztz8cPS8XY",
      "object": "file",
      "bytes": 1547276,
      "created_at": 1610062281,
      "filename": "my-data-train.jsonl",
      "purpose": "fine-tune-train"
    }
  ],
  "updated_at": 1614807789,
}
)";

auto OPENAI_EXAMPLE_FINETUNE_EVENTS = R"(
{
  "object": "list",
  "data": [
    {
      "object": "fine-tune-event",
      "created_at": 1614807352,
      "level": "info",
      "message": "Job enqueued. Waiting for jobs ahead to complete. Queue number: 0."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807356,
      "level": "info",
      "message": "Job started."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807861,
      "level": "info",
      "message": "Uploaded snapshot: curie:ft-acmeco-2021-03-03-21-44-20."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807864,
      "level": "info",
      "message": "Uploaded result files: file-QQm6ZpqdNwAaVC3aSz5sWwLT."
    },
    {
      "object": "fine-tune-event",
      "created_at": 1614807864,
      "level": "info",
      "message": "Job succeeded."
    }
  ]
}
)";

auto OPENAI_EXAMPLE_FINETUNE_DELETE = R"(
{
  "id": "curie:ft-acmeco-2021-03-03-21-44-20",
  "object": "model",
  "deleted": true
}
)";

auto OPENAI_EXAMPLE_MODERATION = R"(
{
  "id": "modr-5MWoLO",
  "model": "text-moderation-001",
  "results": [
    {
      "categories": {
        "hate": false,
        "hate/threatening": true,
        "self-harm": false,
        "sexual": false,
        "sexual/minors": false,
        "violence": true,
        "violence/graphic": false
      },
      "category_scores": {
        "hate": 0.22714105248451233,
        "hate/threatening": 0.4132447838783264,
        "self-harm": 0.005232391878962517,
        "sexual": 0.01407341007143259,
        "sexual/minors": 0.0038522258400917053,
        "violence": 0.9223177433013916,
        "violence/graphic": 0.036865197122097015
      },
      "flagged": true
    }
  ]
}
)";
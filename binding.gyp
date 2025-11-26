{
  "targets": [
    {
      "target_name": "xinput",
      "conditions": [
        ["OS=='win'", {
          "sources": ["src/xinput.cc"],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")"
          ],
          "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
          ],
          "libraries": [
            "xinput.lib"
          ],
          "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"],
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1
            }
          }
        }],
        ["OS!='win'", {
          "sources": ["src/xinput_stub.cc"],
          "include_dirs": [
            "<!@(node -p \"require('node-addon-api').include\")"
          ],
          "dependencies": [
            "<!(node -p \"require('node-addon-api').gyp\")"
          ],
          "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
        }]
      ]
    }
  ]
}


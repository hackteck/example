{
   "targets": [
      {
         "target_name": "emitter",
         "sources": [
            "src/emitter/init.cc"
         ],
         "include_dirs": [
            "src/emitter",
            "<!(node -e \"require('nan')\")"
         ],
         "msvs_settings": {
				"VCCLCompilerTool": {
					"AdditionalOptions": [
						"/std:c++17"
					]
				}
			},
         "msvs_disabled_warnings": []
      }
   ]
}
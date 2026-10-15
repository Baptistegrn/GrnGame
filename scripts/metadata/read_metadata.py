#for open engine we use c wrapper in cjson , but for embedding etc .. we use this script 

import json
import sys

metadata = "metadata.json"

try:
    with open(metadata, "r", encoding="utf-8") as f:
        data = json.load(f)
except FileNotFoundError:
    sys.exit(f"Error: Are you sure {metadata} exists?")
except json.JSONDecodeError:
    sys.exit(f"Error: {metadata} contains invalid JSON.")
except OSError as e:
    sys.exit(f"Error: Could not read {metadata}: {e}")

for key in ("name", "version", "company"):
    if key not in data:
        sys.exit(f"Error: Are you sure '{key}' exists in {metadata}?")

print(data["name"])
print(data["version"])
print(data["company"])
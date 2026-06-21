import sys
import os
import shutil
import subprocess
import zipfile
import tempfile

EXPECTED_RELEASES = [
    "linux-arm64-embedded",
    "linux-arm64-standalone",
    "linux-x64-embedded",
    "linux-x64-standalone",
    "macos-arm64-embedded",
    "macos-arm64-standalone",
    "macos-x64-embedded",
    "macos-x64-standalone",
    #"wasm-embedded",
    "windows-arm64-embedded",
    "windows-arm64-standalone",
    "windows-x64-embedded",
    "windows-x64-standalone"
]

def validate_arguments() -> tuple[str, str]:
    # Require two arguments: the directory and the release version
    if len(sys.argv) != 3:
        print("Format: python check_dependencies.py <directory_path> <release_version>")
        print("Example: python check_dependencies.py ./releases v1.0.11")
        sys.exit(1)
    return sys.argv[1], sys.argv[2]

def get_dyneeded_path() -> str:
    # Search for the command in PATH (handles both dyneeded and dyneeded.exe)
    path = shutil.which("dyneeded")
    if not path:
        path = shutil.which("dyneeded.exe")
    
    if not path:
        print("Critical Error: 'dyneeded' not found in PATH.")
        sys.exit(1)
        
    return path

def find_wrentest(target_dir: str) -> str:
    # Iterate through subdirectories to find the WrenTest binary
    for root, _, files in os.walk(target_dir):
        for file in files:
            if file in ("WrenTest", "WrenTest.exe"):
                return os.path.join(root, file)
    return ""

def parse_release_info(release_name: str) -> tuple[str, str, str]:
    parts = release_name.split("-")
    if len(parts) == 3:
        return parts[0], parts[1], parts[2]
    if len(parts) == 2 and parts[0] == "wasm":
        return parts[0], "N/A", parts[1]
    return "Unknown", "Unknown", "Unknown"

def main() -> None:
    target_dir, release_version = validate_arguments()
    
    if not os.path.isdir(target_dir):
        print(f"Error: Directory '{target_dir}' does not exist.")
        sys.exit(1)
        
    dyneeded_path = get_dyneeded_path()
    missing_releases: list[str] = []
    
    for base_release in EXPECTED_RELEASES:
        # Expected zip filename format
        expected_zip = f"{base_release}-{release_version}.zip"
        zip_full_path = os.path.join(target_dir, expected_zip)
        
        if not os.path.isfile(zip_full_path):
            missing_releases.append(f"{expected_zip} (File not found)")
            continue
            
        # Create a temporary directory to extract the zip contents safely
        with tempfile.TemporaryDirectory() as temp_dir:
            try:
                with zipfile.ZipFile(zip_full_path, 'r') as zip_ref:
                    zip_ref.extractall(temp_dir)
            except zipfile.BadZipFile:
                missing_releases.append(f"{expected_zip} (Corrupted zip file)")
                continue
                
            wrentest_path = find_wrentest(temp_dir)
            if not wrentest_path:
                missing_releases.append(f"{expected_zip} (WrenTest not found inside zip)")
                continue
                
            os_name, arch, config = parse_release_info(base_release)
            print(f"=== OS: {os_name.upper()} | Arch: {arch.upper()} | Config: {config.upper()} | Version: {release_version} ===")
            
            # Execute dyneeded on the extracted binary
            try:
                result = subprocess.run(
                    [dyneeded_path, wrentest_path],
                    capture_output=True,
                    text=True,
                    encoding='utf-8',  # <-- Force l'UTF-8 pour corriger l'affichage des bordures
                    errors='replace',  # <-- Remplace les erreurs s'il reste des caractères bizarres
                    check=False
                )
                
                if result.stdout.strip():
                    print(result.stdout.strip())
                if result.stderr.strip():
                    print("--- Error Output ---")
                    print(result.stderr.strip())
                    
            except Exception as e:
                print(f"Error executing dyneeded: {e}")
                
            print("\n" + "-"*50 + "\n")
        
    if missing_releases:
        print("!!! WARNING: Missing or incomplete images !!!")
        for missing in missing_releases:
            print(f"- {missing}")

if __name__ == '__main__':
    main()
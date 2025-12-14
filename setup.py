from setuptools import setup, find_packages
from setuptools.command.build_py import build_py
from pathlib import Path
import sys
import platform
import os

# os du systeme
CURRENT_OS = platform.system().lower() 
# python version
CURRENT_PYTHON = f"python{sys.version_info.major}.{sys.version_info.minor}"
#print
print(f"Build configuration détectée : {CURRENT_OS} / {CURRENT_PYTHON}")

# class 
class CustomBuild(build_py):
    def find_data_files(self, package, src_dir):
        """
        Cette fonction est appelée pour lister tous les fichiers de données
        avant de les inclure. C'est ici qu'on applique le filtre.
        """
        files = super().find_data_files(package, src_dir)
        
        filtered_files = []
        
        for file_path in files:
            path_parts = Path(file_path).parts
            #suppresion de creation/gif
            if 'creations' in path_parts and 'gif' in path_parts:
                continue 
            # suppression de demo/mp4
            if 'demo' in path_parts and 'mp4' in path_parts:
                continue 
                
            if 'dist' in path_parts:
                try:
                    dist_index = path_parts.index('dist')
                    if len(path_parts) > dist_index + 2:
                        folder_os = path_parts[dist_index + 1]
                        folder_python = path_parts[dist_index + 2]
                        
                        if folder_os != CURRENT_OS or folder_python != CURRENT_PYTHON:
                            continue
                except ValueError:
                    pass 
            filtered_files.append(file_path)
        return filtered_files

this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text(encoding="utf-8")

setup(
    name="GrnGame",
    version="3.0.0",
    author="Baptiste GUERIN",
    author_email="baptiste.guerin34@gmail.com",
    description="GrnGame est un moteur de jeu 2D performant pour Python, conçu spécifiquement pour le pixel art.",
    long_description=long_description,
    long_description_content_type="text/markdown",

    packages=find_packages(include=['GrnGame', 'GrnGame.*']),
    
    # On garde include_package_data pour qu'il scanne tout au début, 
    # mais notre CustomBuild va filtrer ensuite.
    include_package_data=True,
    package_data={"GrnGame": ["**/*"]},

    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Topic :: Games/Entertainment",
    ],
    python_requires=">=3.10",

    entry_points={
        "console_scripts": [
            "GrnGame_app = GrnGame:compilation",
            "GrnGame_xmake = GrnGame.xmake.installation:installation_xmake",
        ],
    },
    
    cmdclass={
        'build_py': CustomBuild,
    },
)
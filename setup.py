from setuptools import setup, find_packages
from pathlib import Path

this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text(encoding="utf-8")

setup(
    name="GrnGame",
    version="2.0.0",
    author="Baptiste GUERIN",
    author_email="baptiste.guerin34@gmail.com",
    description="GrnGame est un moteur de jeu 2D performant pour Python, conçu spécifiquement pour le pixel art.",
    long_description=long_description,
    long_description_content_type="text/markdown",

    packages=find_packages(include=['GrnGame', 'GrnGame.*']),
    include_package_data=True,
    package_data={"GrnGame": ["**/*"]},

    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
        "Topic :: Games/Entertainment",
    ],
    python_requires=">=3.8",

    entry_points={
        "console_scripts": [
            "GrnGame_app = GrnGame:compilation",
            "GrnGame_xmake = GrnGame.xmake.installation:installation_xmake",
        ],
    },
)

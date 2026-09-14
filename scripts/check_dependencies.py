import json
import sys


def get_all_dependencies(file_path: str) -> list:
    with open(file_path, "r", encoding="utf-8") as file:
        data = json.load(file)

    dependencies = []
    def walk(deps):
        for dep in deps:
            dependencies.append(dep)
            walk(dep["deps"])

    walk(data["deps"])

    return dependencies


if __name__ == "__main__":
    file = sys.argv[1]
    dependencies = get_all_dependencies(file)
    #test
    for dependency in dependencies:
        print(dependency["name"])

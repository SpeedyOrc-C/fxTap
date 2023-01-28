import yaml
import sys

def main(path: str):
    with open(path, 'r') as f:
        metadata = yaml.load(f, Loader=yaml.Loader)

    lines = ['#include <gint/display.h>\n\n']
    for file in metadata:
        img_type = metadata[file]['type']
        if img_type != 'bopti-image': continue

        img_id = metadata[file]['name']
        lines.append(f'extern const bopti_image_t {img_id};\n')

    with open('./src/fxconv-assets.h', 'w') as f:
        f.writelines(lines)

    return 0

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Please provide the path to fxconv-metadata.txt')
        exit(1)

    path = sys.argv[1]
    exit(main(path))

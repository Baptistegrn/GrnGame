from PIL import Image, ImageDraw
import sys
import math

CELL_SIZE = 50 

def validate_arguments() -> str:
    if len(sys.argv) != 2:
        print("Format : python generate_palette.py <file_path>")
        sys.exit(1)
    return sys.argv[1]

def count_lines_in_file(filepath: str) -> int:
    with open(filepath, "r", encoding="utf-8") as f:
        return sum(1 for line in f if line.strip())

def calculate_grid_dimension(num_colors: int) -> int:
    return math.ceil(math.sqrt(num_colors))

def create_blank_canvas(grid_dimension: int) -> Image.Image:
    pixel_size = grid_dimension * CELL_SIZE
    return Image.new(
        mode="RGB",
        size=(pixel_size, pixel_size),
        color=(255, 255, 255)
    )

def save_image(image: Image.Image, filename: str = "palette.png") -> None:
    image.save(filename)

def draw_palette_grid(image: Image.Image, filepath: str, grid_dimension: int) -> None:
    draw = ImageDraw.Draw(image)
    x = 0
    y = 0
    index = 0
    
    with open(filepath, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line:
                continue
            r, g, b = map(int, line.split(","))
            x_coord = x * CELL_SIZE
            y_coord = y * CELL_SIZE
        
            draw.rectangle(
                [(x_coord, y_coord), (x_coord + CELL_SIZE, y_coord + CELL_SIZE)],
                fill=(r, g, b)
            )
        
            luminance = (0.299 * r + 0.587 * g + 0.114 * b)
            text_color = (0, 0, 0) if luminance > 128 else (255, 255, 255)
            
            center_x = x_coord + (CELL_SIZE / 2)
            center_y = y_coord + (CELL_SIZE / 2)
            draw.text((center_x, center_y), str(index), fill=text_color, anchor="mm")
            
            index += 1
            x += 1
            
            if x >= grid_dimension:
                x = 0
                y += 1

def main() -> None:
    filepath = validate_arguments()
    num_colors = count_lines_in_file(filepath)
    
    if num_colors == 0:
        print("Error : File empty or invalid.")
        sys.exit(1)
        
    grid_dim = calculate_grid_dimension(num_colors)
    img = create_blank_canvas(grid_dim)
    
    draw_palette_grid(img, filepath, grid_dim)
    save_image(img)

if __name__ == "__main__":
    main()
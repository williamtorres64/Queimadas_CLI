from PIL import Image

def convert_image_to_ascii(image_path, width, height):
    """
    Scales an image, converts it to black and white, and then prints
    an ASCII art representation.

    Args:
        image_path (str): The path to the image file.
        width (int): The desired width for the ASCII art.
        height (int): The desired height for the ASCII art.
    """
    try:
        # Open the image file
        image = Image.open(image_path)
    except FileNotFoundError:
        print(f"Error: The file at {image_path} was not found.")
        return
    except Exception as e:
        print(f"An error occurred: {e}")
        return

    # Resize the image to the specified width and height
    image = image.resize((width, height), Image.Resampling.LANCZOS)

    # Convert the image to black and white (1-bit pixels, black or white)
    # The '1' mode means 1-bit pixels, black and white, with 0 for black and 255 for white.
    image = image.convert('1')

    # Get the pixel data from the image
    pixels = list(image.getdata())
    
    # Iterate through the pixels and build the ASCII art string
    ascii_art = ""
    for i, pixel_value in enumerate(pixels):
        # A black pixel (value 0) is a space, a white pixel (value 255) is a hashtag.
        if pixel_value == 0:
            ascii_art += " "
        else:
            ascii_art += "#"
        
        # Add a newline character at the end of each row
        if (i + 1) % width == 0:
            ascii_art += "\n"

    # Print the final ASCII art
    print(ascii_art)

# Example usage:
# Replace 'acre-mapa-estado-do-brasil-ilustracao-vetorial_601298-7743.jpg' with the path to your image
image_file_path = "mapa.jpg"
new_width = 80
new_height = 40

print(f"Converting '{image_file_path}' to ASCII art with dimensions {new_width}x{new_height}:\n")
convert_image_to_ascii(image_file_path, new_width, new_height)


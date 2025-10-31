import csv
from PIL import Image, ImageDraw
import random

# Step 1: Read lat, lon, and estadoId from CSV
points = []
with open("estado_2324/ACRE.csv", newline="", encoding="utf-8") as f:
    reader = csv.DictReader(f)
    for row in reader:
        lat = float(row["lat"])
        lon = float(row["lon"])
        estado_id = int(row["estadoId"])
        points.append((lat, lon, estado_id))

# Step 2: Shift so no value is below 0
min_lat = min(lat for lat, lon, _ in points)
min_lon = min(lon for lat, lon, _ in points)
shifted = [(lat - min_lat, lon - min_lon, estado_id)
           for lat, lon, estado_id in points]

# Step 3: Multiply by 1000 and round to integers
scaled = [(round(lat * 1000), round(lon * 1000), estado_id)
          for lat, lon, estado_id in shifted]

# Step 4: Determine image size
max_lat = max(lat for lat, lon, _ in scaled)
max_lon = max(lon for lat, lon, _ in scaled)
width = max_lat + 11
height = max_lon + 11

# Step 5: Assign random colors for each estadoId
estado_colors = {}
for _, _, estado_id in scaled:
    if estado_id not in estado_colors:
        estado_colors[estado_id] = (
            random.randint(40, 200),
            random.randint(40, 200),
            random.randint(40, 200)
        )

# Step 6: Create white image
img = Image.new("RGB", (width, height), "white")
draw = ImageDraw.Draw(img)

# Step 7: Draw colored circles (north-up)
radius = 10
for lat, lon, estado_id in scaled:
    x = lat
    y = lon  # flip vertically for north-up orientation
    color = estado_colors[estado_id]
    draw.ellipse([x - radius, y - radius, x + radius, y + radius], fill=color)



# Step 8: Save image
# Scale down the image by 10 times
# new_width = (max_lat + 11) // 10
# new_height = height // 10
# img = img.resize((new_width, new_height))
# Rotate the image 90 degrees counter-clockwise
# img = img.rotate(90)
img = img.rotate(90, expand=True)
img.save("mapa.png")
print("mapa.png created successfully!")

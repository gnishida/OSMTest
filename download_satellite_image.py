from PIL import Image, ImageFile
import requests
import argparse
import math
import time

LON_STEP = 0.000858
LAT_STEP = 0.00067
TILE_WIDTH = 640
TILE_HEIGHT = 640
TILE_CROPPED_HEIGHT = 615

ImageFile.LOAD_TRUNCATED_IMAGES = True

def download_image(lat, lon, key):
    url = "https://maps.googleapis.com/maps/api/staticmap?center={},{}&zoom=20&size={}x{}&format=png&maptype=satellite&key={}".format(lat, lon, TILE_WIDTH, TILE_HEIGHT, key)
    return Image.open(requests.get(url, stream=True).raw)   

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--min_lon", type=float, default=139.9195000, help = "minimum longitude")
    parser.add_argument("--max_lon", type=float, default=139.9420000, help = "maximum longitude")
    parser.add_argument("--min_lat", type=float, default=35.6361000, help = "minimum latitude")
    parser.add_argument("--max_lat", type=float, default=35.6512000, help = "maximum latitude")
    parser.add_argument("--key", required=True, help = "Google API key")
    args = parser.parse_args()
        
    num_y_steps = int(math.ceil((args.max_lat - args.min_lat) / LAT_STEP))
    num_x_steps = int(math.ceil((args.max_lon - args.min_lon) / LON_STEP))
    
    y_extra = int((LAT_STEP * num_y_steps - (args.max_lat - args.min_lat)) / LAT_STEP * TILE_CROPPED_HEIGHT)
    x_extra = int((LON_STEP * num_x_steps - (args.max_lon - args.min_lon)) / LON_STEP * TILE_WIDTH)
    
    image_width = TILE_WIDTH * num_x_steps - x_extra
    image_height = TILE_CROPPED_HEIGHT * num_y_steps - y_extra
    
    image = Image.new("RGB", (image_width, image_height))
    
    for y_index in range(num_y_steps):
        lat1 = args.max_lat - LAT_STEP * y_index
        lat2 = args.max_lat - LAT_STEP * (y_index + 1)
        if lat1 <= args.min_lat:
            break
        y = TILE_CROPPED_HEIGHT * y_index
        
        for x_index in range(num_x_steps):
            lon1 = args.min_lon + LON_STEP * x_index
            lon2 = args.min_lon + LON_STEP * (x_index + 1)
            if lon1 >= args.max_lon:
                break
            x = TILE_WIDTH * x_index
            
            print("downloading {}: ({}, {})".format(cnt, (lat1 + lat2) * 0.5, (lon1 + lon2) * 0.5))
            img = download_image((lat1 + lat2) * 0.5, (lon1 + lon2) * 0.5, args.key)
            image.paste(img, (x, y))
            time.sleep(3)
            
            cnt += 1
            if cnt % 5 == 0:
                while True:
                    try:
                        image.save("__download.png")
                    except:
                        print("retrying...")
                        continue
                    break
            
    
    while True:
        try:
            image.save("download.png")
        except:
            print("retrying...")
            continue
        break

            
if __name__== "__main__":
    main()

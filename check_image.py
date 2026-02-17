import os

file_path = r"C:\Users\Yeni\Desktop\DİL\Client\pack\locale\locale\tr\ui\login_medium.jpg"

if not os.path.exists(file_path):
    print("File not found")
else:
    size = os.path.getsize(file_path)
    print("Size: {}".format(size))
    if size > 10:
        with open(file_path, "rb") as f:
            header = f.read(10)
            print("Header: {}".format(header.hex()))
            f.seek(-2, 2)
            footer = f.read(2)
            print("Footer: {}".format(footer.hex()))
    else:
        print("File too small")

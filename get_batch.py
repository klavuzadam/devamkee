# -*- coding: utf-8 -*-
import json
import sys

def get_batch(start_id, count):
    try:
        with open('locale_pairs.json', 'r', encoding='utf-8') as f:
            pairs = json.load(f)
    except Exception as e:
        print(f"Error: {e}")
        return

    batch = pairs[start_id : start_id + count]
    for p in batch:
        # Avoid terminal encoding issues by using backslashreplace
        key_str = repr(p['key']).encode('ascii', 'backslashreplace').decode('ascii')
        pl_str = repr(p['pl']).encode('ascii', 'backslashreplace').decode('ascii')
        print(f"ID: {p['id']}")
        print(f"KEY: {key_str}")
        print(f"PL: {pl_str}")
        print("-" * 20)

if __name__ == "__main__":
    start = int(sys.argv[1]) if len(sys.argv) > 1 else 0
    num = int(sys.argv[2]) if len(sys.argv) > 2 else 50
    get_batch(start, num)

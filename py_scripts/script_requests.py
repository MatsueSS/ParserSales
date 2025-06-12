from datetime import datetime
import requests
import json
import sys

# url = "https://5d.5ka.ru/api/catalog/v2/stores/5156/categories/251C17046/products?mode=delivery&include_restrict=true&limit=120"

headers = {
    'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:138.0) Gecko/20100101 Firefox/138.0',
    'Accept': 'application/json, text/plain, */*',
    'Accept-Language': 'ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3',
    'Accept-Encoding': 'gzip, deflate, br, zstd',
    'X-DEVICE-ID': '085d2704-2206-4623-8cad-a7a90341c7d9',
    'X-APP-VERSION': '0.1.1.dev',
    'X-PLATFORM': 'webapp',
    'Origin': 'https://5ka.ru',
    'DNT': '1',
    'Sec-GPC': '1',
    'Connection': 'keep-alive',
    'Sec-Fetch-Dest': 'empty',
    'Sec-Fetch-Mode': 'cors',
    'Sec-Fetch-Site': 'same-site',
    'TE': 'trailers',
}

low = 1
high = 512
best_data = None

while low <= high:
    mid = (low + high) // 2
    url = f"https://5d.5ka.ru/api/catalog/v2/stores/Y232/categories/251C17045/products?mode=delivery&include_restrict=true&limit={mid}"
    response = requests.get(url, headers=headers)

    if response.status_code == 422:
        high = mid
        continue

    if response.status_code == 403:
        sys.exit(1)

    new_data = response.json()
    products = new_data.get("products")

    if products is None:
        high = mid - 1
    else:
        if best_data is None:
            best_data = new_data
            low = mid + 1
            continue
        if len(products) == len(best_data.get("products")):
            break
        else:
            best_data = new_data
            low = mid + 1

today_str = datetime.now().strftime("%d-%m-%Y")

cards = []

data = best_data

for product in data.get("products", []):
    name = product.get("name", "")
    prices = product.get("prices", {})

    regular = prices.get("regular")
    discount = prices.get("discount")

    cards.append({
        "name": name,
        "price": regular,
        "sale": discount
    })

result = {
    "date": today_str,
    "cards": cards
}

with open("../res/cards.json", "w", encoding="utf-8") as f:
    json.dump(result, f, ensure_ascii=False, indent=4)

sys.exit(0)
from selenium import webdriver
from selenium_stealth import stealth
from datetime import datetime
from bs4 import BeautifulSoup

import json
import sys
import time

def init_webdriver():
    driver = webdriver.Chrome()
    stealth(driver,
            platform="Win32",
            languages=["en-US", "en"],
            vendor="Google Inc.",
            webgl_vendor="Intel Inc.",
            renderer="Inrel Iris OpenGL Engine") 
    return driver

def scrolldown(driver, deep):
    for _ in range(deep):
        driver.execute_script("window.scrollBy(0, 500)")
        time.sleep(0.7)

driver = init_webdriver()
driver.get("https://5ka.ru/catalog/skidki-nedeli--251C17046/")
time.sleep(20)
scrolldown(driver, 200)
time.sleep(100)

html = driver.page_source

soup = BeautifulSoup(html, "html.parser")

cards = []

today_str = datetime.now().strftime("%d-%m-%Y")

html_value = soup.find_all("div", class_="productFilterGrid_cardContainer__oyUJZ css-0")
for card in html_value:
    name = card.find("p", class_="chakra-text mainInformation_title__ziiEa css-1jdqp4k")
    price = card.find("p", class_="chakra-text priceContainer_catalogUsualPrice__m97fW css-bx74j9")
    sale = card.find("p", class_="chakra-text priceContainer_price__AY8C_ css-6uvdux")

    if name and sale and price:
        cards.append({
            "name": name.text,
            "price": price.text,
            "sale": sale.text
        })

result = {
    "date": today_str,
    "cards": cards
}

with open("../res/cards.json", "w", encoding="utf-8") as f:
    json.dump(result, f, ensure_ascii=False, indent=4)


driver.quit()

sys.exit(0)
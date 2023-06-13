# WIP, selenium does not support post so for now abandoned for the sake of request.
# eitherway client is implemented using c++

from selenium import webdriver
from selenium.webdriver.chrome.service import Service
from selenium.webdriver.chrome.options import Options
from utils.get_container_ip import get_server_app_docker_containers
# Set Chrome options
chrome_options = Options()
chrome_options.add_argument("--headless")  # Optional: Run Chrome in headless mode

# Set Chrome driver service
driver_service = Service("http://localhost:4444/wd/hub")  # Replace with the path to your chromedriver executable

# Create a WebDriver instance with Chrome options and service
driver = webdriver.Chrome(service=driver_service, options=chrome_options)


#driver.get("https://www.google.com")
ip = get_server_app_docker_containers()
address = ":".join([ip, "8000"])
driver.get(address)
driver.get("http://127.17.0.2:8000")
print(driver.title)

driver.get("http://127.17.0.2:8000/test")
print(dir(driver))
# Close the browser
driver.quit()

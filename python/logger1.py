
# https://docs.python.org/ko/3/library/logging.html

import logging

#logging.basicConfig()

# 로거 설정
logger = logging.getLogger('MyLogger')
logger.setLevel(logging.INFO)

# 핸들러 및 포맷터 설정
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
stream_handler = logging.StreamHandler()
stream_handler.setFormatter(formatter)

# FileHandler 설정
file_handler = logging.FileHandler('my.log', mode='a')
file_handler.setLevel(logging.DEBUG)
file_handler.setFormatter(formatter)

logger.addHandler(stream_handler)
logger.addHandler(file_handler)

# 로그 기록
logger.info('정보 로그')
logger.warning('경고 로그')


run:
	(cat event1.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event2.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event3.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event4.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event5.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event6.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event7.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event8.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event9.txt | python3 mapper1.py |sort -n | python3 combiner.py \
	& cat event10.txt | python3 mapper1.py |sort -n | python3 combiner.py) \
	| sort -n | python3 reducer1.py |python3 mapper2.py|python3 reducer2.py |python3 mapper3.py|python3 reducer3.py | sort -n >result_event.txt
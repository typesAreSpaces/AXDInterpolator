files.zip: memsafety_done reachsafety_done
	rm -rf files
	mkdir files
	mv MemSafety-Arrays ReachSafety-Arrays *.txt files
	zip -r 'files.zip' files

memsafety_done:
	rm -rf memsafety_done
	time ./generate-experiments-for-MemSafety-Array.sh
	touch memsafety_done

reachsafety_done:
	rm -rf reachsafety_done
	time ./generate-experiments-for-ReachSafety-Array.sh
	touch reachsafety_done

clean:
	rm -rf MemSafety-Arrays ReachSafety-Arrays
	rm -rf memsafety_done
	rm -rf reachsafety_done
	rm -rf *.txt

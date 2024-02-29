echo "Program zacina"

for E in {150..350..20}; do
	echo "$E MeV "
	sed  -i "s|/gun/energy.*|/gun/energy $E MeV|" run1.mac
	./Bragg -m run1.mac
	cp Bragg.root out_MeV_$E.root
done

	

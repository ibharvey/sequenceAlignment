#removeDuplicates.py

#pull all sequences
def getFasta(infile):
	#pull the fasta data
	frl = open(infile,'r').readlines()
	names = []
	seqs = []
	tempSeq = ''
	for line in frl:
		if '>' in line:
			names.append(line)
			seqs.append(tempSeq)
			tempSeq = ''
		if not '>' in line:
			tempSeq = tempSeq + line
	seqs.append(tempSeq)
	seqs.pop(0)
	#get rid of carriage returns
	for i in range(0,len(seqs)):
		names[i] = names[i].replace('\n','')
		seqs[i] = seqs[i].replace('\n','')
	return [names,seqs]

#can remove duplicate sequences or duplicate names: 
#	specified by boolean argument
def removeDuplicates(infile, repeatSeqs = True, outfile = 'output.fasta'):
	[names,seqs]=getFasta(infile)
	#iterate through and find only nonRepeats
	iter = 0
	nonRepeatSeqs = []
	nonRepeatNames = []
	while iter < len(seqs):
		isRepeat = False
		if repeatSeqs:
			for nrs in nonRepeatSeqs:
				if seqs[iter]==nrs:
					isRepeat = True
					break
		else:
			for nrn in nonRepeatNames:
				if names[iter]==nrn:
					isRepeat = True
					break
		if not isRepeat:
			nonRepeatSeqs.append(seqs[iter])
			nonRepeatNames.append(names[iter])
		iter+=1
	printNice(outfile, nonRepeatSeqs, nonRepeatNames)
	
def printNice(outfile, seqs, names):
	fw = open(outfile,'w+')
	for i in range(0,len(seqs)):
		#write each name
		if names[i][-1]=='\n':
			fw.write(names[i])
		else:
			fw.write(names[i]+'\n')
		#write each sequence 60 letters at a time
		j = 0
		while j < len(seqs[i]):
			fw.write(seqs[i][j:j+70]+'\n')
			j+=70
		fw.write('\n')
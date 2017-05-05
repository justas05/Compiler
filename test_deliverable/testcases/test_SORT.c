void sort(int *array, int size)
{
	int swapped=1;
	
	while(swapped)
	{
		int i;
		
		swapped=0;
		for(i=1; i<size; ++i)
		{
			if(array[i-1]>array[i])
			{
				int tmp_el=array[i-1];
				
				array[i-1]=array[i];
				array[i]=tmp_el;
				swapped=1;
			}
		}
	}
}

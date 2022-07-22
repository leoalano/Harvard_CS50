 //new locking
    for (int x = 0; x < pair_count; x++)
    {
        int nocycle = 0;

        for (int k = x; k > -1; k--)
        {
            int counter = 0;
            for (int s = 0; s < x + 1; s++)
            {
                if (pairs[k].winner != pairs[s].loser)
                {
                    counter = counter + 1;
                }
            }

            if (counter == x + 1)
            {
                nocycle = nocycle + 1;
            }
        }
        if (nocycle != 0)
        {
            locked[pairs[x].winner][pairs[x].loser] = true;
        }
        else
        {
            locked[pairs[x].winner][pairs[x].loser] = false;
        }
    }

    return;
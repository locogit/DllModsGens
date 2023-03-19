using System;
using System.Collections.Generic;

namespace ConfigurationTool.Helpers
{
    public static class Extensions
    {
        public static void InsertElementDescending<T>(this List<T> source,
        T toAdd) where T : IComparable
        {
            int index = source.FindLastIndex(elem => elem.CompareTo(toAdd) > 0);
            source.Insert(index + 1, toAdd);
        }

        public static void InsertElementAscending<T>(this List<T> source,
T toAdd) where T : IComparable
        {
            int index = source.FindIndex(elem => elem.CompareTo(toAdd) < 0);
            source.Insert(index + 1, toAdd);
        }
    }
}

#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Очистка OSM-файла от объектов с action='delete'.
Сохраняет копию файла с суффиксом _cleaned.osm (или заданным именем).
"""

import sys
from lxml import etree


def clean_osm(input_file: str, output_file: str = None) -> None:
    """
    Читает OSM-файл, удаляет все node/way/relation с action='delete',
    и сохраняет результат.
    """
    if output_file is None:
        # Если выходной файл не указан, добавляем _cleaned перед расширением
        if input_file.endswith('.osm'):
            output_file = input_file.replace('.osm', '_cleaned.osm')
        else:
            output_file = input_file + '_cleaned.osm'

    # Парсим XML, сохраняя структуру
    parser = etree.XMLParser(remove_blank_text=True)
    tree = etree.parse(input_file, parser)
    root = tree.getroot()

    # Находим все элементы с action="delete" (без учёта регистра – но в файле всегда delete)
    # Используем XPath для выборки
    for elem in root.xpath('//node[@action="delete"] | //way[@action="delete"] | //relation[@action="delete"]'):
        parent = elem.getparent()
        if parent is not None:
            parent.remove(elem)

    # Записываем обратно с красивым форматированием
    with open(output_file, 'wb') as f:
        f.write(etree.tostring(tree, pretty_print=True, encoding='UTF-8', xml_declaration=True))

    print(f"Очищенный файл сохранён: {output_file}")


if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Использование: python clean_osm.py <входной.osm> [выходной.osm]")
        sys.exit(1)

    input_path = sys.argv[1]
    output_path = sys.argv[2] if len(sys.argv) > 2 else None
    clean_osm(input_path, output_path)
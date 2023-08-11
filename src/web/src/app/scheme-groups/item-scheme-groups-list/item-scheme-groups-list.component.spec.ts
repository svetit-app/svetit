import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ItemSchemeGroupsListComponent } from './item-scheme-groups-list.component';

describe('ItemSchemeGroupsListComponent', () => {
  let component: ItemSchemeGroupsListComponent;
  let fixture: ComponentFixture<ItemSchemeGroupsListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ItemSchemeGroupsListComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ItemSchemeGroupsListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});

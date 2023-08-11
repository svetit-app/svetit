import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SchemeGroupsListComponent } from './scheme-groups-list.component';

describe('SchemeGroupsListComponent', () => {
  let component: SchemeGroupsListComponent;
  let fixture: ComponentFixture<SchemeGroupsListComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SchemeGroupsListComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SchemeGroupsListComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
